/*
    #include <sys/types.h>
    #include <sys/socket.h>

    ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);
    函数功能：向网络上的其它 socket 发送数据（可面向无连接）
    函数参数
    - sockfd：通信用的文件描述符
    - buf：要发送数据的字符串数组
    - len：发送数据的长度
    - flags：默认使用 0 即可
    - dest_addr：网络通信另外一端的地址信息（目的端的 socket 信息）
    - addrlen：参数 dest_addr 所占内存的大小
    返回值
    - 成功：返回发送数据的字节数
    - 失败：返回 -1 并且设置错误号

    ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
    函数功能：接收网络上其它 socket 发送过来的数据（可面向无连接）
    函数参数
    - sockfd：通信用的文件描述符
    - buf：接收数据的字符串数组
    - len：数组的大小
    - flags：默认使用 0 即可
    - src_addr：网络通信另外一端的地址信息（源端的 socket 信息）
    - addrlen：参数 src_addr 所占内存的大小
    返回值
    - 成功：返回接收数据的字节数
    - 失败：返回 -1 并且设置错误号
*/
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

    // 2. 绑定 ip + port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;   // 接收所有网络接口的 UDP 数据报
    server_addr.sin_port = htons(9999);
    int ret1 = bind(communication_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret1 == -1) {
        perror("bind");
        exit(-1);
    }

    // 3. 进行通信
    while (1) {
        // 接收客户端发来的 UDP 数据报
        char receive_buf[128] = { '\0' };
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        char client_ip[16] = { '\0' };
        unsigned short client_port = 0;

        int ret2 = recvfrom(communication_fd, receive_buf, sizeof(receive_buf), 0, (struct sockaddr*)&client_addr, &addr_len);
        if (ret2 == -1) {
            perror("recvfrom");
            exit(-1);
        }
        else if (ret2 == 0) {
            printf("client closed.\n");
            break;
        }

        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
        ntohs(client_addr.sin_port);

        // 输出客户端信息 + 接收的数据
        printf("client ip = %s, port = %u is sending data: %s.\n", client_ip, client_port, receive_buf);

        // 向客户端发送数据
        int ret3 = sendto(communication_fd, receive_buf, sizeof(receive_buf), 0, (struct sockaddr*)&client_addr, addr_len);
        if (ret3 == -1) {
            perror("sendto");
            exit(-1);
        }

    }

    // 4. 关闭通信用的文件描述符
    close(communication_fd);

    return 0;
}