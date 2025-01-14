#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main() {
    // 1. 创建 socket
    int communication_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (communication_fd == -1) {
        perror("socket...");
        exit(0);
    }
    // 2. 连接服务器端，创建 sockaddr_in 对象
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", (void*)&server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(9999);
    int ret1 = connect(communication_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (ret1 == -1) {
        perror("connect");
        exit(0);
    }

    // 3. 通信
    int i = 0;
    while (1) {
        char receive_buf[1024] = { '\0' };
        char send_buf[1024] = { '\0' };
        //sleep(1);
        fgets(send_buf, sizeof(send_buf), stdin);
        //sprintf(send_buf, "process id = %d, send data: %d", getpid(), i);

        write(communication_fd, send_buf, sizeof(send_buf));  // 向服务器端发送数据

        int ret2 = read(communication_fd, receive_buf, sizeof(receive_buf));    // 读取服务器端发送的数据
        if (ret2 > 0) {
            printf("receive_server_buf: %s", receive_buf);
        }
        else if (ret2 == 0) {
            // 在 TCP 通信中，等于 0 的情况表示服务器端断开连接
            printf("server closed\n");
            break;
        }
        else if (ret2 == -1) {
            perror("read");
            exit(0);
        }
        ++i;
    }

    // 4. 关闭文件描述符
    close(communication_fd);

    return 0;
}