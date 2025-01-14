/*
    #include <unistd.h>

    unsigned int alarm(unsigned int seconds);

    函数功能：设置定时器，函数调用，开始倒计时，当倒计时为 0 的时候，
             函数会给当前的进程发送一个信号：SIGALRM
    函数参数
    - seconds：定时器时长，单位：秒，如果参数为0，定时器无效（不进行倒计时）
        - 可以通过alarm(0)取消一个定时器
    返回值
    - alarm()之前没有定时器，返回0
    - alarm()之前有定时器，返回之前的定时器剩余的时间

    alarm()不阻塞进程，定时结束之后，给调用的进程 SIGALRM 信号，默认终止进程（定时的进行和进程状态没关系）

*/




#include<unistd.h>
#include<stdio.h>

int main(int argc, char* argv[]) {
    int seconds = alarm(5);
    printf("seconds = %d\n", seconds);

    sleep(2);
    seconds = alarm(2);
    printf("seconds = %d\n", seconds);

    while (1) {}
    return 0;
}