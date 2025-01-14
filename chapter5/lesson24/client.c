#include<stdio.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>



/*
    TCP 通信客户端实现思路
*/
int main(int argc, char* argv[]) {
    // 1. 创建 socket 对应的 fd
    int communication_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (communication_fd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2. 向客户端发送连接请求
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    inet_pton(AF_INET, "172.30.192.45", (void*)&server_addr.sin_addr.s_addr);

    int ret1 = connect(communication_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret1 == -1) {
        perror("connect");
        exit(-1);
    }

    // 3. 通信
    while (1) {
        char send_buf[1024] = { '\0' };
        char receive_buf[1024] = { '\0' };
        fgets(send_buf, sizeof(send_buf), stdin);
        write(communication_fd, send_buf, sizeof(send_buf));
        int ret2 = read(communication_fd, receive_buf, sizeof(receive_buf));
        if (ret2 == -1) {
            perror("read");
            exit(-1);
        }
        else if (ret2 == 0) {
            // 服务器断开连接
            printf("server closed\n");
            break;
        }
        else if (ret2 > 0) {
            printf("receive server data: %s", receive_buf);
        }
    }

    // 4. 关闭文件描述符
    close(communication_fd);
    return 0;
}