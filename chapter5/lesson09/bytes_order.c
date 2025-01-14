/*
    测试 PC 机是大端模式还是小端模式，可以尝试使用对字符数组的赋值来测试
    - 大端模式：整数高位存放在内存低地址
    - 小端模式：整数高位存放在内存高地址
*/

#include<stdio.h>

int main(int argc, char* argv[]) {
    unsigned int u_num = 0x12345678;

    // 定义一个字符指针指向 u_num 所在的内存
    char* c_num = (char*)&u_num;

    // c_num 指向低地址，c_num + 1 指向高地址
    printf("c_num[0] = %x \nc_num[1] = %x\n", *c_num, *(c_num + 1));

    if (*c_num == 0x78) {
        printf("小端模式\n");
    }
    else if (*c_num == 0x12) {
        printf("大端模式\n");
    }

    return 0;
}