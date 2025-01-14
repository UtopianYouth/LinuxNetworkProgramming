#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>

int main(int argc, char* argv[]) {
    pid_t pid = fork();
    if (pid > 0) {
        while (1) {
            //父进程死循环，子进程执行结束后，内核区的空间不能得以释放，占用PCB，成为僵尸进程
            printf("parent process, pid = %d, ppid = %d\n", getpid(), getppid());
            sleep(1);
        }
    }
    else if (pid == 0) {
        printf("child process, pid = %d, ppid = %d\n", getpid(), getppid());
        sleep(1);
    }
    else {
        printf("create child process failure....\n");
    }

    //for循环
    for (int i = 0;i < 5;++i) {
        printf("i = %d, pid = %d, ppid = %d\n", i, getpid(), getppid());
    }
    return 0;
}