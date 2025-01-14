#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(int argc, char* argv[]) {
    // 1. 创建 UDP 通信用的 socket , PF_INET 的使用更加关注协议族，AF_INET 的使用更加关注地址族
    int communication_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (communication_fd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2. 设置通信的 socket 广播属性
    int optval = 1;     // 允许广播
    setsockopt(communication_fd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));

    // 3. 设置广播的 IP + port
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "172.30.207.255", &client_addr.sin_addr.s_addr);
    client_addr.sin_port = htons(9999);

    socklen_t addr_len = sizeof(client_addr);

    // 4. 进行通信，发送广播消息
    int num = 0;
    while (1) {
        sleep(1);
        char send_buf[128] = { '\0' };
        sprintf(send_buf, "I am server, num = %d.", ++num);
        // 向客户端发送数据
        int ret3 = sendto(communication_fd, send_buf, sizeof(send_buf), 0, (struct sockaddr*)&client_addr, addr_len);
        if (ret3 == -1) {
            perror("sendto");
            exit(-1);
        }
        printf("%s\n", send_buf);
    }

    // 5. 关闭通信用的文件描述符
    close(communication_fd);

    return 0;
}