/*
    本地套接字通信流程（服务器端）
*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/un.h>

int main(int argc, char* argv[]) {
    // 1. 创建监听用的 socket
    int listen_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
    }

    // 2. 将 socket 绑定一个虚拟文件（相当于网络间通信绑定服务器端的网络接口）
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, "server.sock");

    // 绑定虚拟磁盘文件之前，删除已经存在的虚拟磁盘文件
    int ret4 = unlink("./server.sock");
    if (ret4 == -1) {
        printf("not exist server.sock.\n");
    }

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

    // 4. 接受本地客户端发送的连接请求
    struct sockaddr_un client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int communication_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (communication_fd == -1) {
        perror("accept");
        exit(-1);
    }
    printf("client connect success, sun_path = %s.\n", client_addr.sun_path);

    // 5. 通信
    while (1) {
        char recv_buf[128] = { '\0' };
        int ret3 = read(communication_fd, recv_buf, sizeof(recv_buf));
        if (ret3 == -1) {
            perror("read");
            exit(-1);
        }
        else if (ret3 == 0) {
            printf("client closed.\n");
            break;
        }
        else {
            printf("receive client data: %s\n", recv_buf);
            write(communication_fd, recv_buf, sizeof(recv_buf));
        }
    }

    // 6. 关闭文件描述符
    close(communication_fd);
    close(listen_fd);
    return 0;
}
