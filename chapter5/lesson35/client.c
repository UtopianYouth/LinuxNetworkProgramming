/*
    本地套接字通信流程（客户端）
*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<string.h>

int main(int argc, char* argv[]) {
    // 1. 创建通信用的 socket
    int communication_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (communication_fd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2. 通信的 socket 绑定本地的虚拟文件
    struct sockaddr_un client_addr;
    client_addr.sun_family = AF_LOCAL;
    strcpy(client_addr.sun_path, "client_sock");

    // 绑定虚拟磁盘文件之前，删除已经存在的虚拟磁盘文件
    int ret4 = unlink("./client.sock");
    if (ret4 == -1) {
        printf("not exist client.sock.\n");
    }
    int ret3 = bind(communication_fd, (struct sockaddr*)&client_addr, sizeof(client_addr));
    if (ret3 == -1) {
        perror("bind");
        exit(-1);
    }

    // 3. 连接本地套接字（服务器端）
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, "server.sock");
    int ret1 = connect(communication_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret1 == -1) {
        perror("connect");
        exit(-1);
    }

    // 4. 通信
    int count = 0;
    while (1) {
        sleep(1);
        char send_buf[128] = { '\0' };
        char recv_buf[128] = { '\0' };
        sprintf(send_buf, "I am client, count = %d.", count++);
        write(communication_fd, send_buf, sizeof(send_buf));
        int ret2 = read(communication_fd, recv_buf, sizeof(recv_buf));
        if (ret2 == -1) {
            perror("read");
            exit(-1);
        }
        else if (ret2 == 0) {
            printf("server closed.\n");
            break;
        }
        else {
            printf("receive server data: %s\n", recv_buf);
        }
    }

    // 5. 关闭文件描述符
    close(communication_fd);

    return 0;
}
