#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include<string.h>
#include<unistd.h>

/*
    #include <sys/types.h>
    #include <signal.h>

    int kill(pid_t pid, int sig);
    功能：给任何进程或者进程组pid, 发送任何的信号 sig
    函数参数
    - pid：
        - 大于0：信号发送给指定进程
        - 等于0：信号发送给当前进程组
        - 等于-1：信号发送给每一个有权限接收该信号的进程
        - 小于-1：信号发送给进程组ID = -pid 的进程
    - sig：需要发送的信号编号或者是宏值
        - 等于0：不发送任何信号
    返回值
    - 0：成功
    - -1：失败


*/

/*
    #include<signal.h>

    int raise(int sig);
    函数功能：给当前进程发送信号
    函数参数
    - sig：需要发送的信号编号或者是宏值
        - 等于0：不发送任何信号
    返回值
    - 0：成功
    - 非0：失败

    等价于 kill(getpid(),sig);
*/

/*
    #include <stdlib.h>

    void abort(void);
    函数功能：发送SIGABRT信号给当前的进程，杀死当前进程

    等价于 kill(getpid(), SIGABRT);
*/

int main(int argc, char* argv[]) {
    pid_t pid = fork();

    if (pid == 0) {
        // 子进程
        int i;
        for (i = 0;i < 5;++i) {
            printf("child process\n");
            sleep(1);
        }
    }
    else if (pid > 0) {
        // 父进程
        printf("parent process\n");
        sleep(2);
        printf("kill child process now\n");
        kill(pid, SIGINT);
    }
    return 0;
}