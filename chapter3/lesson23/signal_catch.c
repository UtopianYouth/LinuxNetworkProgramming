/*
    #include <signal.h>

    typedef void (*sighandler_t)(int);      // 定义了一个名为sighandler_t，返回值为void，参数列表为int的函数指针
    sighandler_t signal(int signum, sighandler_t handler);

    函数功能：设置某一个信号的捕捉之后的行为
    函数参数
    - signum：要捕捉的信号
    - handler：对捕捉到的信号要如何处理
        - SIG_IGN：忽略捕捉到的信号
        - SIG_DEL：使用信号默认的行为
        - 自定义回调函数
            - 内核调用，开发人员自定义函数规则
            - 函数指针是实现回调的手段，函数名表示的就是函数所在内存空间中的地址

    返回值
    - 成功：返回上一次信号处理函数的地址，第一次调用，返回NULL
    - 失败：返回SIG_ERR，设置错误号
*/
#include <sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

/*
    通过setitimer实现 2s 的定时，间隔 3s 执行

    通过signal捕捉信号，实现周期定时
*/
void my_alarm(int sig_num) {
    printf("The number of catching signal is %d.\n", sig_num);
}

int main(int argc, char* argv[]) {

    // 注册信号捕捉
    //signal(SIGALRM, SIG_DFL);   // 信号默认行为
    //signal(SIGALRM, SIG_IGN);   // 忽略信号的默认行为
    signal(SIGALRM, my_alarm);        // 自定义回调函数处理信号

    // 创建定时器参数
    struct itimerval new_value;

    // 设置定时器参数
    new_value.it_interval.tv_sec = 0;       // 定时器执行间隔时间，设置为0，定时器setitimer只执行一次
    new_value.it_interval.tv_usec = 0;

    new_value.it_value.tv_sec = 3;          // 设置每一次定时的有效时间
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
    }

    return 0;
}