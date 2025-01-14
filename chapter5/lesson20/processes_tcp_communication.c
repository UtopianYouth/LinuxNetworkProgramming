/*
    TCP 通信服务器处理并发的任务，使用多线程和多进程来解决

    基本思路：
        1. 一个父进程，多个子进程
        2. 父进程：负责等待并且接受客户端的连接
        3. 子进程：完成通信，接受一个客户端连接，就创建一个子进程用于通信
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<signal.h>
#include<wait.h>
#include<errno.h>

/*
    关于子进程资源如何回收的问题
    - 多进程实现 TCP 通信服务器端，由于父进程一直处于运行状态（不断监听新的 TCP 连接），
      而子进程只负责通信，当通信结束后，子进程也结束运行，但是子进程资源一直没有被释放
        - 回收方法1：在父进程逻辑中调用 wait 函数，不可取，因为 wait 阻塞会影响监听
        - 回收方法2：在父进程逻辑中调用 waitpid函数，也不可取，因为 accept 函数会阻塞，影响子进程回收
        - 回收方法3：使用信号机制，一旦有子进程运行结束，父进程捕捉信号，回收子进程资源，可取
*/
void recycle_child_process(int arg) {
    while (1) {
        // 调用一次 waitpid 回收一个子进程资源
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret == -1) {
            // 所有子进程都被回收了
            break;
        }
        else if (ret == 0) {
            // 子进程正在运行
            break;
        }
        else if (ret > 0) {
            printf("recycle child process, pid = %d\n", ret);
        }
    }
}


// TCP 服务器端父进程
int main(int argc, char* argv[]) {

    // 0. 使用信号机制，回收子进程资源
    struct sigaction act;
    act.sa_flags = 0;   // 使用 act.sa_handler 对应的函数
    sigemptyset(&act.sa_mask);      // 清空信号集掩码，表示不临时阻塞任何信号
    act.sa_handler = recycle_child_process;
    sigaction(SIGCHLD, &act, NULL); // 注册信号捕捉

    // 1. 创建监听客户端连接用的 socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2. 给监听客户端的 socket 绑定 IP 和 port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);             // 指定服务器监听的端口号
    server_addr.sin_addr.s_addr = INADDR_ANY;       // 监听任意 IP 地址
    int ret1 = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret1 == -1) {
        perror("bind");
        exit(-1);
    }

    // 3. 监听客户端发来的连接请求
    int ret2 = listen(listen_fd, 10);
    if (ret2 == -1) {
        perror("listen");
    }

    pid_t pid = 0;

    // 4. 父进程不断接受客户端发来的 TCP 连接
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = (socklen_t)sizeof(client_addr);

        // 该函数会阻塞，直到有客户端发来了连接请求（监听文件描述符对应的 socket 监听到 TCP 连接），才会执行
        int communication_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len);
        if (communication_fd == -1) {
            if (errno == EINTR) {
                // accept 系统调用会被捕捉到的信号中断
                continue;
            }
            perror("accept");
            exit(-1);
        }
        printf("client connect success.\n");

        pid = fork();
        if (pid > 0) {
            // 父进程
        }
        else if (pid == 0) {
            // 获取客户端信息
            char client_ip[16] = { '\0' };
            unsigned short client_port = 0;
            inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
            client_port = ntohs(client_addr.sin_port);
            printf("pid = %d, client_ip = %s, client_port = %u.\n", getpid(), client_ip, client_port);

            // 5. 子进程与客户端进行通信
            while (1) {
                char receive_data[1024] = { '\0' };
                int ret3 = read(communication_fd, receive_data, sizeof(receive_data));
                if (ret3 == 0) {
                    // 客户端断开连接
                    printf("client closed.\n");
                    break;
                }
                else if (ret3 > 0) {
                    printf("pid = %d, receive client data: %s\n", getpid(), receive_data);
                    write(communication_fd, receive_data, sizeof(receive_data));
                }
                else {
                    perror("read");
                    exit(-1);
                }
            }

            // 6. 子进程关闭通信文件描述符
            close(communication_fd);
            exit(0);
        }
        else {
            perror("fork");
            break;
        }
    }

    // 7. 父进程释放监听用的文件描述符
    if (pid > 0) {
        close(listen_fd);
    }

    return 0;
}