/*

    在进行网络通信时，需要将主机字节序转换为网络字节序（大端）

    #include <arpa/inet.h>

    字节序转换函数
    - h：host，主机字节序
    - to：从...转换成...
    - n：network，网络字节序
    - s：unsigned short 类型序列
    - long：unsigned int 类型序列


    // 转换端口
    uint16_t htons(uint16_t hostshort);
    函数功能：将 unsigned short 类型变量从主机字节序（不确定是否是大端还是小端）转换为网络字节序（大端）
    函数参数
    - hostshort：需要转换的无符号短整型数
    返回值：转换后的结果

    uint16_t ntohs(uint16_t netshort);
    函数功能：将 unsigned short 类型变量从网络字节序（大端）转换为主机字节序（不确定是否是大端还是小端）
    函数参数
    - hostshort：需要转换的无符号短整型数
    返回值：转换后的结果

    // 转换 IP
    uint32_t htonl(uint32_t hostlong);
    函数功能：将 unsigned int 类型变量从主机字节序（不确定是否是大端还是小端）转换为网络字节序（大端）
    函数参数
    - hostshort：需要转换的无符号整型数
    返回值：转换后的结果

    uint32_t ntohl(uint32_t netlong);
    函数功能：将 unsigned int 类型变量从网络字节序（大端）转换为主机字节序（不确定是否是大端还是小端）
    函数参数
    - hostshort：需要转换的无符号整型数
    返回值：转换后的结果


*/

#include<stdio.h>
#include<arpa/inet.h>
#include<stdlib.h>

int main() {
    unsigned short num1 = 0x0102;
    // 主机字节序转换为网络字节序（大端）
    unsigned short num2 = htons(num1);
    // 网络字节序转换为主机字节序
    unsigned short num3 = ntohs(num2);

    printf("source num = %x, htons() num = %x, ntohs() num = %x\n", num1, num2, num3);

    unsigned char buf1[4] = { 192,168,0,100 };
    // 定义两个指针，接收转换后的IP地址
    unsigned int* buf2 = (unsigned int*)malloc(sizeof(unsigned int));
    unsigned int* buf3 = (unsigned int*)malloc(sizeof(unsigned int));

    // 主机字节序转换为网络字节序
    *buf2 = htonl(*(uint32_t*)buf1);
    // 网络字节序转换为主机字节序
    *buf3 = ntohl(*buf2);

    // 无符号整数，printf() 的 format 为 %u
    unsigned char* p = (char*)buf1;
    printf("source IP: %u.%u.%u.%u\n", *p, *(p + 1), *(p + 2), *(p + 3));

    p = (char*)buf2;
    printf("htonl() IP: %u.%u.%u.%u\n", *p, *(p + 1), *(p + 2), *(p + 3));

    p = (char*)buf3;
    printf("ntohl() IP: %u.%u.%u.%u\n", *p, *(p + 1), *(p + 2), *(p + 3));

    return 0;

}