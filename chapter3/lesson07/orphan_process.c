#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>

/*
    argc和argv都是用来处理命令行的参数，属于可执行文件的参数列表
    argc: 是一个整数，表示传递给程序的命令行参数数量
    argv: 字符指针数组，包含了所有命令行参数的值，每一个元素都是指向字符串首地址的指针
*/
int main(int argc, char* argv[]) {
    int num = 10;
    pid_t pid = fork();
    // 判断父子进程
    if (pid > 0) {
        printf("parent process, pid = %d, ppid = %d\n", getpid(), getppid());
    }
    else if (pid == 0) {
        printf("child process, pid = %d, ppid = %d\n", getpid(), getppid());
        sleep(10);
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