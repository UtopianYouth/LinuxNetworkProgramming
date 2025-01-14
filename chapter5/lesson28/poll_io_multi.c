// poll IO 多路复用（转接）代码的编写
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/poll.h>


int main(int argc, char* argv[]) {
    // 监听socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        exit(-1);
    }

    // 绑定 IP + port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    // 监听
    ret = listen(listen_fd, 128);
    if (ret == -1) {
        perror("listen");
        exit(-1);
    }

    // 初始化监听的文件描述符集合
    struct pollfd fds[1024];
    int nfds = 0;
    for (int i = 0;i < 1024;++i) {
        fds[i].fd = -1;
        fds[i].events = POLLIN;     // 委托内核检测文件描述符读缓冲区内容
    }


    fds[0].fd = listen_fd;
    int fds_len = sizeof(fds) / sizeof(fds[0]);
    ++nfds;

    // 并发处理与客户端的 TCP 通信
    while (1) {
        // 委托内核检测文件描述符对应的缓冲区是否有内容
        int ret1 = poll(fds, nfds, -1);
        if (ret1 == 0) {
            // 没有可操作的文件描述符
            continue;
        }
        else if (ret1 == -1) {
            perror("poll");
            exit(-1);
        }
        else {
            if ((fds[0].revents & POLLIN) == POLLIN) {
                // 有新的客户端连接进来
                struct sockaddr_in client_addr;
                socklen_t addr_len = sizeof(client_addr);
                int communication_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &addr_len);
                if (communication_fd == -1) {
                    perror("accept");
                    exit(-1);
                }
                printf("connect success.\n");

                unsigned short client_port = htons(client_addr.sin_port);
                char client_ip[16] = { '\0' };
                inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
                printf("client_ip = %s, client_port = %u.\n", client_ip, client_port);

                // 将 communication_fd 加入到文件描述符集合中
                for (int i = 1;i < fds_len;++i) {
                    if (fds[i].fd == -1) {
                        // 找到一个可用的文件描述符
                        fds[i].fd = communication_fd;
                        fds[i].events = POLLIN;
                        if (i > nfds - 1) {
                            // 更新最大文件描述符集合索引，这里有技巧性，因为可能中间索引的文件描述符先被释放
                            ++nfds;
                        }
                        // 找到一个可用的文件描述符，记得退出循环
                        break;
                    }
                }
            }

            // 通信
            for (int i = 1;i < nfds;++i) {
                if ((fds[i].revents & POLLIN == POLLIN) && (fds[i].fd != -1)) {
                    char receive_buf[1024] = { '\0' };
                    int ret2 = read(fds[i].fd, receive_buf, sizeof(receive_buf));
                    if (ret2 == -1) {
                        perror("read");
                        exit(-1);
                    }
                    else if (ret2 == 0) {
                        // 客户端连接断开
                        printf("client close\n");
                        close(fds[i].fd);
                        fds[i].fd = -1;
                        fds[i].events = POLLIN;
                    }
                    else if (ret2 > 0) {
                        printf("receive data: %s\n", receive_buf);
                        write(fds[i].fd, receive_buf, sizeof(receive_buf));
                    }
                }
            }
        }
    }

    // 关闭监听用的文件描述符
    close(listen_fd);

    return 0;
}