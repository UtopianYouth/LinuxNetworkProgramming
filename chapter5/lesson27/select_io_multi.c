/*
    select IO 多路复用（转接）代码的编写
*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/time.h>


int main(int argc, char* argv[]) {
    // 1. 创建监听用的 socket，得到 fd
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2. 将监听用的 fd 绑定服务器端的 IP & port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    // 3. 监听客户端发来的连接请求
    ret = listen(listen_fd, 128);
    if (ret == -1) {
        perror("listen");
        exit(-1);
    }

    // 初始化文件描述符集合
    fd_set readfds, tmp;
    FD_ZERO(&readfds);

    FD_SET(listen_fd, &readfds);

    int max_fd = listen_fd;

    // 4. 接受客户端发来的连接请求
    while (1) {

        /*
            tmp 变量防止 select() 函数覆盖 readfds 中的内容，因为客户端可能会停止发送数据，但是客户端并没有断开连接，
            如果 readfds 传入 select() 函数，读缓冲区数据为空时，对应的 fd 标志位会被置 0 （内核的文件描述符集合会重
            新赋值给传入 select() 的文件描述符集合变量）
        */
        tmp = readfds;

        // ret 存储了 select 统计的 tmp 集合中值为 1 的个数
        int ret1 = select(max_fd + 1, &tmp, NULL, NULL, NULL);
        if (ret1 == -1) {
            perror("select");
            exit(-1);
        }
        else if (ret1 > 0) {
            // 有新的客户端连接进来，如果没有新的客户端连接，listen_fd 的读缓冲区没有内容，tmp[listen_fd]的值会被内核设置为 0
            if (FD_ISSET(listen_fd, &tmp)) {        // 注意这里使用 tmp 而不是 readfds，readfds[listen_fd] 的值恒为 1
                struct sockaddr_in client_addr;
                socklen_t addr_len = sizeof(client_addr);
                int communication_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &addr_len);
                if (communication_fd == -1) {
                    perror("accept");
                    exit(-1);
                }
                printf("connect success.\n");
                // 获取客户端信息
                char client_ip[16] = { '\0' };
                inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
                unsigned short client_port = ntohs(client_addr.sin_port);
                printf("client ip = %s, port = %u.\n", client_ip, client_port);

                // 将对应的通信文件描述符加入到文件描述符集合中
                FD_SET(communication_fd, &readfds);
                max_fd = communication_fd > max_fd ? communication_fd : max_fd;
            }

            // 5. 与客户端进行通信，这里通过 select() 实现的 I/O 多路复用，并发的处理多个客户端的 TCP 通信
            for (int i = listen_fd + 1;i <= max_fd;++i) {
                // 从监听文件描述符后一位遍历，一般的，监听文件描述符的值最小
                if (FD_ISSET(i, &tmp)) {
                    char receive_data[1024] = { '\0' };
                    int ret3 = read(i, receive_data, sizeof(receive_data));     // 如果 TCP 连接有效，对应的读缓冲区没有数据，该函数会阻塞
                    if (ret3 == 0) {
                        printf("client closed.\n");
                        close(i);
                        // 完成通信，移除对应的文件描述符
                        FD_CLR(i, &readfds);
                    }
                    else if (ret3 > 0) {
                        printf("receive client data: %s\n", receive_data);
                        // 将接收的内容返回给客户端
                        write(i, receive_data, sizeof(receive_data));
                    }
                    else if (ret3 == -1) {
                        perror("read");
                        exit(-1);
                    }
                }
            }
        }
        else if (ret1 == 0) {
            // 读缓冲区文件描述符集合，不存在 bit 位变化，实际上这个分支不可能执行到，当 ret3 == 0 时，select() 会阻塞            
            continue;
        }
    }

    // 关闭监听文件描述符
    close(listen_fd);
    return 0;
}




// #include<stdio.h>
// #include<arpa/inet.h>
// #include<string.h>
// #include<stdlib.h>
// #include<string.h>
// #include<sys/select.h>
// #include<unistd.h>


// int main() {
//     // 创建 socket
//     int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (listen_fd == -1) {
//         perror("socket");
//         exit(-1);
//     }
//     // 绑定
//     struct sockaddr_in server_addr;
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(9999);
//     server_addr.sin_addr.s_addr = INADDR_ANY;       // 表示使用服务器端的任意 IP 地址，因为服务器可能有多网卡，或者 DHCP 服务导致 IP 地址动态变化
//     int ret1 = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
//     if (ret1 == -1) {
//         perror("bind");
//         exit(-1);
//     }
//     // 监听
//     int ret2 = listen(listen_fd, 128);  // 已完成三次握手的连接队列和未完成三次握手的未连接队列总和
//     if (ret2 == -1) {
//         perror("listen");
//         exit(-1);
//     }
//     // 创建一个 fd_set 文件描述符集合，存放需要检测的文件描述符
//     fd_set readfds, tmp;

//     // 初始化 fd_set，并且设置指定文件描述符值为 1
//     FD_ZERO(&readfds);
//     FD_SET(listen_fd, &readfds);

//     int max_fd = listen_fd;

//     // 接受客户端发来的连接请求
//     while (1) {
//         /*
//             tmp 变量防止 select() 函数覆盖 readfds 中的内容，因为客户端可能会停止发送数据，但是
//             客户端并没有断开连接，如果 readfds 传入 select() 函数，读缓冲区数据为空时，对应的fd
//             标志位会被置 0，导致无法操作 fd 对应的读缓冲区内容
//         */
//         tmp = readfds;

//         //printf("readfds[listen_fd] = %d, tmp[listen_fd] = %d\n", FD_ISSET(listen_fd, &readfds), FD_ISSET(listen_fd, &tmp));

//         // select() 系统调用函数，让内核帮忙检测哪些文件描述符对应的读缓冲区有数据，阻塞函数
//         int ret3 = select(max_fd + 1, &tmp, NULL, NULL, NULL);

//         //printf("select readfds[listen_fd] = %d, tmp[listen_fd] = %d\n", FD_ISSET(listen_fd, &readfds), FD_ISSET(listen_fd, &tmp));

//         if (ret3 == -1) {
//             perror("select");
//             exit(-1);
//         }

//         else if (ret3 > 0) {
//             // 有新的客户端连接进来
//             if (FD_ISSET(listen_fd, &tmp)) {
//                 char client_ip[16] = { '\0' };
//                 unsigned short port = 0;
//                 struct sockaddr_in client_addr;
//                 socklen_t client_addr_len = (socklen_t)sizeof(client_addr);
//                 int communication_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
//                 if (communication_fd == -1) {
//                     perror("accept");
//                     exit(-1);
//                 }

//                 // 获取客户端信息
//                 inet_ntop(AF_INET, (void*)&client_addr.sin_addr.s_addr, client_ip, (socklen_t)sizeof(client_ip));
//                 port = ntohs(client_addr.sin_port);
//                 printf("client ip = %s, port = %d\n", client_ip, port);

//                 // 将对应的通信文件描述符加入到文件描述符集合中
//                 FD_SET(communication_fd, &readfds);

//                 max_fd = max_fd > communication_fd ? max_fd : communication_fd;
//             }

//             // 通信
//             // 从监听文件描述符后一位遍历，一般的，监听文件描述符的值最小
//             for (int i = listen_fd + 1;i <= max_fd;++i) {
//                 if (FD_ISSET(i, &readfds)) {
//                     // 存在文件描述符对应的 bit 标志位为 1，说明客户端发来了消息
//                     char receive_buf[1024] = { '\0' };
//                     int ret4 = read(i, receive_buf, sizeof(receive_buf));
//                     if (ret4 == -1) {
//                         perror("read");
//                         exit(-1);
//                     }
//                     else if (ret4 == 0) {
//                         printf("client closed\n");
//                         close(i);
//                         FD_CLR(i, &readfds);    // 完成通信，移除对应的文件描述符
//                     }
//                     else if (ret4 > 0) {
//                         printf("receive data of client: %s\n", receive_buf);
//                         write(i, receive_buf, sizeof(receive_buf));
//                     }
//                 }
//             }
//         }

//         else if (ret3 == 0) {
//             // 读缓冲区文件描述符集合，不存在 bit 位变化，实际上这个分支不可能执行到，当 ret3 == 0时，select() 会阻塞
//             continue;
//         }
//     }

//     // 关闭文件描述符
//     close(listen_fd);

//     return 0;
// }