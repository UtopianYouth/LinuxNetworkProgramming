/*
    IP 地址转换函数（字符串IP - 整数）

    #include <arpa/inet.h>

    int inet_aton(const char *cp, struct in_addr *inp);

    in_addr_t inet_addr(const char *cp);
    函数功能：
    函数参数
    返回值

    char *inet_ntoa(struct in_addr in);


    // 下面这对更新的函数也能完成前面三个函数同样的功能，并且同时适用 IPv4 和 IPv6

    int inet_pton(int af, const char *src, void *dst);
    - p 表示点分十进制的 IP 字符串，n 表示网络字节序
    函数功能：将点分十进制的 IP 字符串转换为网络字节序的二进制形式
    函数参数
    - af：需要转换协议族（地址族），常见的 AF_INET( IPv4 ) 和 AF_INET6( IPv6 )
    - src：需要转换的 IP 地址字符串
    - dst：存储转换后网络字节序的二进制结果
    返回值
    - 转换成功：返回 1
    - 转换失败：返回 0，表示 src 对于 af 协议族来说，不是一个有效的 IP 地址
    - 转换失败：返回 -1，表示 af 不是一个有效的 IP 地址协议族

    const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
    函数功能：将网络字节序的二进制形式转换为点分十进制 IP 地址文本形式
    函数参数
    - af：需要转换协议族（地址族），常见的 AF_INET( IPv4 ) 和 AF_INET6( IPv6 )
    - src：需要转换的二进制网络字节序
    - dst：存储转换后的 IP 文本字符串结果
    - size：dst 字符数组的大小
    返回值
    - 转换成功：返回转换后 IP 字符串的地址，和 dst 的值一样
    - 转换失败，返回 NULL，并且设置错误号

*/

#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>

int main(int argc, char* argv[]) {
    // 点分十进制 IP 转换为 网络字节序二进制
    char src_ip[] = "192.168.1.3";
    unsigned int num = 0;
    inet_pton(AF_INET, src_ip, (void*)&num);
    unsigned char* p = (char*)&num;
    printf("%d.%d.%d.%d\n", *p, *(p + 1), *(p + 2), *(p + 3));

    // 网络字节序二进制 转换为 点分十进制 IP, 4*3 + 3 + 1 = 16
    char* dst_ip = (char*)malloc(16 * sizeof(char));
    const char* dst_ip_addr = inet_ntop(AF_INET, (void*)&num, dst_ip, 16);
    printf("%s\n", dst_ip);
    printf("%d\n", dst_ip == dst_ip_addr);

    return 0;
}