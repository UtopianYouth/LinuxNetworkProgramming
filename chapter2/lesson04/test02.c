#include<stdio.h>

// 若程序使用该宏，则输出该语句
#ifdef DEBUG
    printf("DEBUG is defined\n");
#endif

int main() {

    int a, b, c, d;

    a = 3;
    b = a;
    c = b;
    d = c;


    for (int i = 0;i < a;++i) {
        printf("hello gcc...\n");
    }
    return 0;
}