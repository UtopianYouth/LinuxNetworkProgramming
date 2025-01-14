/*
    #include <sys/time.h>

    int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);

    函数功能：间隔定时器，定时器时间到，向调用定时器的进程发送终止信号，可以替代alarm函数，可以实现周期定时

    函数参数
    - which：定时器以什么时间计时
        - ITIMER_REAL：定时器记录进程的真实时间（进程运行的所有阶段时间，包括IO，用户态，核心态等），时间到达，发送 SIGALRM 信号
        - ITIMER_VIRTUAL：定时器记录进程占用CPU的使用时间，时间到达，发送 SIGVTALRM
        - ITIMER_PROF：定时器记录进程占用CPU的时间和系统调用的时间，时间到达，发送 SIGPROF
    - new_value：设置定时器的属性

        struct itimerval {      // 定时器结构体
            struct timeval it_interval;     // 每个阶段的间隔时间，即每个定时器执行的间隔时间
            struct timeval it_value;        // 延迟多长时间执行定时器
        };

        struct timeval {        // 时间结构体
            time_t      tv_sec;     // 秒
            suseconds_t tv_usec;    // 微秒
        };

    - old_value：记录上一次的定时器时间参数，一般不使用，赋值NULL即可



    返回值
    - 定时器设置成功，返回 0
    - 失败，返回 -1

*/
#include <sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

/*
    定时器是非阻塞的

    通过setitimer实现 2s 的定时，间隔 3s 执行
*/
int main(int argc, char* argv[]) {

    // 创建定时器参数
    struct itimerval new_value;

    // 设置定时器参数
    new_value.it_interval.tv_sec = 3;       // 定时器执行间隔时间
    new_value.it_interval.tv_usec = 0;

    new_value.it_value.tv_sec = 5;          // 设置每一次定时的有效时间
    new_value.it_value.tv_usec = 0;

    // 创建定时器，非阻塞
    int ret = setitimer(ITIMER_REAL, &new_value, NULL);
    printf("setitimer is starting...\n");

    if (ret == -1) {
        perror("setitimer");
        exit(0);
    }

    while (1) {
        sleep(1);
        printf("setitimer is working.....\n");
    }

    return 0;
}