/*
    #include <signal.h>

    int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
    - 函数功能：将自定义信号集中的数据设置到内核中（设置阻塞，解除阻塞，替换）
    - 函数参数
        - how：如何对内核阻塞信号集进行处理
            - SIG_BLOCK：将用户设置的阻塞信号集添加到内核中，内核中原来的数据不变
                假设用户自定义信号集是 set，内核中阻塞信号集是 mask， mask = mask | set
            - SIG_UNBLOCK：根据用户设置的数据，对内核中的数据进行解除阻塞
                假设用户自定义信号集是 set，内核中阻塞信号集是 mask， mask = mask & (~set)
            - SIG_SETMASK：覆盖内核中原来的值

        - set：用户自定义的阻塞信号集
        - oldset：备份（存储）修改之前的内核中阻塞信号集内容
    - 返回值
        - 成功：0
        - 失败：-1，设置错误号，EFAULT 或 EINVAL

    int sigpending(sigset_t *set);
    - 功能：获取内核中未决信号集，pending -> adj.未决定的
    - 参数
        - set：传出参数，保存的是内核中未决信号集中的信息
    - 返回值
        - 成功：0
        - 失败：-1，设置错误号，EFAULT
*/


/*
    编写一个程序，把所有常规信号（1 ~ 31）的未决状态打印出来
    - 设置某些信号是阻塞的，通过键盘产生这些信号
*/
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char* argv[]) {
    // 自定义阻塞信号集
    sigset_t my_set;

    sigemptyset(&my_set);

    // 设置由键盘产生的2、3号信号阻塞
    sigaddset(&my_set, SIGINT);
    sigaddset(&my_set, SIGQUIT);

    // 将自定义信号集设置到内核阻塞信号集中
    sigprocmask(SIG_BLOCK, &my_set, NULL);

    int num = 0;
    while (1) {

        ++num;

        // 获取内核中未决状态信号集的内容
        int ret = sigpending(&my_set);

        if (ret == -1) {
            perror("sigpending");
            exit(0);
        }

        sleep(1);

        // 访问未决信号集中的内容
        for (int i = 1;i <= 31;++i) {
            printf("%d", sigismember(&my_set, i));
        }
        printf("\n");

        if (num >= 10) {
            // 解除阻塞信号集，对哪一个信号解除阻塞，哪一个信号位置就置1
            sigdelset(&my_set, SIGINT);     //对SIGINT信号集还是阻塞
            sigprocmask(SIG_UNBLOCK, &my_set, NULL);
        }
    }

    return 0;
}