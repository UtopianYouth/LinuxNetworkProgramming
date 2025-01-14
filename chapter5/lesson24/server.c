#include<stdio.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>


/*
    TCP 通信服务器端实现思路
*/
int main(int argc, char* argv[]) {
    // 1. 创建 socket 对应的 fd
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        exit(-1);
    }
    // 设置端口复用
    int optval = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    // 2. 绑定 IP + PORT
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    int ret1 = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret1 == -1) {
        perror("bind");
        exit(-1);
    }

    // 3. 监听
    int ret2 = listen(listen_fd, 128);
    if (ret2 == -1) {
        perror("listen");
        exit(-1);
    }

    // 4. 接受客户端的连接
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int communication_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (communication_fd == -1) {
        perror("accept");
        exit(-1);
    }
    printf("client connect success.\n");

    // 输出连接客户端的信息
    char client_ip[16];
    unsigned short port = 0;
    inet_ntop(AF_INET, (void*)&client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
    port = ntohs(client_addr.sin_port);
    printf("client ip = %s, port = %u.\n", client_ip, port);

    // 5. 循环实现通信
    while (1) {
        char receive_buf[1024] = { '\0' };
        int ret3 = read(communication_fd, receive_buf, sizeof(receive_buf));
        if (ret3 == -1) {
            perror("read");
            break;
        }
        else if (ret3 == 0) {
            // 客户端断开连接
            printf("client closed\n");
            break;
        }
        else if (ret3 > 0) {
            printf("receive data of client ip = %s, port = %d: %s", client_ip, port, receive_buf);
            write(communication_fd, receive_buf, sizeof(receive_buf));
        }
    }

    // 6. 关闭文件描述符资源
    close(listen_fd);
    close(communication_fd);

    return 0;
}