#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>


// 客户端接收服务器广播发来的消息
int main(int argc, char* argv[]) {
    // 1. 创建 UDP 通信用的 socket
    int communication_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (communication_fd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2. 客户端绑定所有网络接口的 IP 和 port
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(9999);
    client_addr.sin_addr.s_addr = INADDR_ANY;
    int ret1 = bind(communication_fd, (struct sockaddr*)&client_addr, sizeof(client_addr));
    if (ret1 == -1) {
        perror("bind");
        exit(-1);
    }

    // 2. 加入多播组
    struct ip_mreq optval;
    optval.imr_interface.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET, "239.0.0.8", &optval.imr_multiaddr.s_addr);
    setsockopt(communication_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &optval, sizeof(optval));

    // 3. 接收客户端发来的广播消息
    while (1) {
        char receive_buf[128] = { '\0' };
        // 接收数据，默认和 read 一样，是阻塞的
        int ret2 = recvfrom(communication_fd, receive_buf, sizeof(receive_buf), 0, NULL, NULL);
        if (ret2 == -1) {
            perror("recvfrom");
            exit(-1);
        }
        else if (ret2 == 0) {
            printf("server closed.\n");
            break;
        }
        printf("receive server data: %s\n", receive_buf);
    }

    // 4. 关闭文件描述符
    close(communication_fd);

    return 0;
}