#include<unistd.h>
#include<stdio.h>


/*
    测试计算机 1s 的数数能力
*/
int main(int argc, char* argv[]) {
    int seconds = alarm(1);
    printf("seconds = %d\n", seconds);

    int i = 0;
    while (1) {
        printf("%d\n", ++i);
    }
    return 0;
}