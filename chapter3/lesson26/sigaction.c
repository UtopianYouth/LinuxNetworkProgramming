/*
    #include <signal.h>
    int sigaction(int signum, const struct sigaction *act,
                    struct sigaction *oldact);

    函数功能：和 signal() 函数一样，进行信号捕捉，并且可以修改信号对应的行为

    函数参数
    - signum：捕捉到的信号编号，建议使用宏值来表示
    - act：捕捉到信号之后的处理动作
    - oldact：上一次对信号捕捉相关的设置，一般不使用，传递NULL

    返回值
    - 成功：0
    - 失败：-1

    struct sigaction {
        // 函数指针，指向的函数就是信号捕捉到之后的行为
        void     (*sa_handler)(int);

        // 不常用
        void     (*sa_sigaction)(int, siginfo_t *, void *);

        // 临时阻塞信号集，信号捕捉函数执行过程中，临时阻塞某些信号
        sigset_t   sa_mask;

        // 指定捕捉到的信号行为
        // 值为 0，表示使用 sa_handler，值为 SA_SIGINFO 表示使用 sa_sigaction
        int        sa_flags;

        // 被废弃，不需要指定
        void     (*sa_restorer)(void);
    };

*/

#include <sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

/*
    通过 setitimer 实现 2s 的定时，间隔 3s 执行

    通过 sigaction 捕捉信号，实现周期定时
*/
void my_alarm(int sig_num) {
    printf("The number of catching signal is %d.\n", sig_num);
}

int main(int argc, char* argv[]) {

    // 注册信号捕捉
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = my_alarm;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);

    // 创建定时器参数
    struct itimerval new_value;

    // 设置定时器参数
    new_value.it_interval.tv_sec = 3;       // 定时器执行间隔时间，设置为0，定时器setitimer只执行一次
    new_value.it_interval.tv_usec = 0;

    new_value.it_value.tv_sec = 2;          // 设置每一次定时的有效时间
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