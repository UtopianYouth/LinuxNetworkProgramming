/*
    SIGCHILD 信号产生的3个条件
    - 子进程结束
    - 子进程暂停
    - 子进程从暂停到继续运行

    都会给父进程发送该信号，父进程默认忽略该信号
*/

#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>

void my_func(int num) {
    printf("SIGCHLD num is %d\n", num);
    //wait(NULL);     //由于信号捕捉机制，wait不能完全回收所有的僵尸进程
    while (1) {
        int ret = waitpid(-1, NULL, WNOHANG);
        if (ret > 0) {
            // 回收一个子进程
            printf("child process die, pid = %d\n", ret);
        }
        else if (ret == 0) {
            // 还存在子进程没有运行结束
            break;
        }
        else if (ret == -1) {
            // 所有子进程都运行结束，且没有可以回收的子进程资源
            break;
        }
    }
}

int main(int argc, char* argv[]) {

    // 提前设置好阻塞信号集，阻塞SIGCHLD，因为有可能子进程很快结束，父进程没有注册完信号捕捉
    sigset_t my_set;
    sigemptyset(&my_set);
    sigaddset(&my_set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &my_set, NULL);

    pid_t pid;

    // 创建一些子进程
    for (int i = 0;i < 20;++i) {
        pid = fork();
        if (pid == 0) {
            break;      //子进程退出循环，防止嵌套创建子进程
        }
    }

    if (pid > 0) {

        // 捕捉子进程死亡时发送的 SIGCHLD 信号
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = my_func;
        sigaction(SIGCHLD, &act, NULL);

        // 父进程
        while (1) {
            // 注册完信号捕捉以后，解除阻塞
            sigprocmask(SIG_UNBLOCK, &my_set, NULL);
            sleep(1);
            printf("parent process pid: %d\n", getpid());
        }
    }
    else if (pid == 0) {
        // 子进程
        printf("child process pid: %d\n", getpid());
    }

    return 0;
}