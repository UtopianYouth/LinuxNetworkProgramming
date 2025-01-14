/*
    多线程实现 TCP 通信，并发服务器

    基本思路：
    - 创建 socket，生成对应的 fd
    - 将 fd 绑定 IP 地址 + port
    - 监听客户端发送的连接请求
    - 接受客户端发送的连接请求（多线程实现并发，因为可能同时有多个客户端）
    - 通信（利用多线程技术）
    - 释放资源
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<string.h>


// 子线程逻辑
void* child_pthread(void* arg) {
    int communication_fd = *((int*)arg);
    // 与客户端进行通信
    while (1) {
        char receive_buf[1024] = { '\0' };
        int ret = read(communication_fd, receive_buf, sizeof(receive_buf));
        if (ret == 0) {
            // 客户端断开 TCP 连接
            printf("client close.\n");
            break;
        }
        else if (ret == -1) {
            perror("read");
            close(communication_fd);
            exit(-1);
        }
        else {
            printf("tid = %ld receive client data: %s\n", pthread_self(), receive_buf);
            write(communication_fd, receive_buf, sizeof(receive_buf));
        }
    }
    close(communication_fd);
    return NULL;
}

int main(int argc, char* argv[]) {
    // 1. 创建 socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        exit(0);
    }

    // 2. 绑定 IP + PORT
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = INADDR_ANY;       // 监听任意 IP 地址的客户端
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);

    int ret1 = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret1 == -1) {
        perror("bind");
        exit(0);
    }

    // 监听客户端发来的连接请求
    int ret2 = listen(listen_fd, 128);
    if (ret2 == -1) {
        perror("listen");
        exit(0);
    }

    // 利用多线程技术，并发处理客户端发来的连接请求
    while (1) {
        struct sockaddr_in client_addr;     // 存储客户端的 socket 信息
        socklen_t len = sizeof(client_addr);
        int communication_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len);
        if (communication_fd == -1) {
            perror("accept");
            exit(0);
        }
        printf("connect success.\n");

        // 获取客户端的信息
        char client_ip[16] = { '\0' };
        unsigned short client_port = ntohs(client_addr.sin_port);
        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
        printf("client ip = %s, client port = %u.\n", client_ip, client_port);

        // 创建子线程，用于处理客户端的连接请求
        pthread_t tid;
        pthread_create(&tid, NULL, child_pthread, &communication_fd);

        // 设置线程分离
        pthread_detach(tid);

    }
    // 关闭监听用的文件描述符
    close(listen_fd);
    return 0;
}