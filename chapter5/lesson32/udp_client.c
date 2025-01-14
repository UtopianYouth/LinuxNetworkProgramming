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
    // 1. 创建 UDP 通信用的 socket
    int communication_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (communication_fd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2. 指定数据接收方的 socket (这里是服务器)
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(9999);

    // 3. 通信
    int count = 1;
    while (1) {
        sleep(1);

        char send_buf[128] = { '\0' };
        char receive_buf[128] = { '\0' };
        sprintf(send_buf, "I am client, count = %d", count++);
        // 发送数据
        int ret1 = sendto(communication_fd, send_buf, sizeof(send_buf), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if (ret1 == -1) {
            perror("sendto");
            exit(-1);
        }

        // 接收数据
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

    // 4. 关闭 UDP 通信用的文件描述符
    close(communication_fd);

    return 0;
}