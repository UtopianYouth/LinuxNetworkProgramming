/*
    #include <sys/types.h>
    #include <sys/wait.h>

    pid_t wait(int *wstatus);
    函数功能：等待任意一个子进程结束，如果任意一个子进程结束了，该函数会回收子进程的所有资源

    参数：int* wstatus，子进程退出时的状态信息，传入的是一个int类型的地址，传出参数

    返回值
    - 成功：返回被回收的子进程id
    - 失败：-1（所有子进程都结束了且没有可以回收的子进程资源，或者是调用函数失败）

    wait函数对父进程状态的影响
    - 调用wait函数的父进程会被挂起（阻塞），直到它的一个子进程结束或者收到一个不可忽略的信号才被唤醒
    - 没有子进程，立刻返回-1，父进程从阻塞态变为就绪态
*/
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[]) {

    pid_t pid;
    //创建5个子进程
    for (int i = 0;i < 5;++i) {
        pid = fork();
        if (pid == 0) {     //防止子进程再次创建子进程，孙子进程再次创建子进程（会一直嵌套执行下去）......
            break;
        }
        else if (pid < 0) {
            printf("create child process failure...\n");
        }
    }
    if (pid > 0) {
        // 父进程
        while (1) {
            printf("parent process, pid = %d\n", getpid());
            int status;
            int ret = wait(&status);       //调用wait阻塞函数，一直阻塞等待子进程结束

            if (ret == -1) {
                printf("no child process need recycle\n");
                break;
            }
            if (WIFEXITED(status)) {
                //是不是正常退出
                printf("退出的状态码：%d\n", WEXITSTATUS(status));
            }
            if (WIFSIGNALED(status)) {
                //是不是异常终止
                printf("异常终止状态码：%d\n", WTERMSIG(status));
            }
            printf("child process die, child pid = %d\n", ret);
            sleep(1);
        }
    }
    else if (pid == 0) {
        printf("child process, pid = %d\n", getpid());
        sleep(1);
    }
    return 0;
}