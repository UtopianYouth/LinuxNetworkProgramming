/*
    man 2 fork 查看源码

    #include<sys/types.h>
    #include<unistd.h>

    pid_t fork(void);
    创建子进程

    返回值：
    创建子进程成功之后，fork的返回值会返回两次，一次是在父进程中，一次是在子进程中
    - 在父进程中返回创建子进程的PID
    - 在子进程中返回0
    - 通过fork的返回值区分父进程和子进程
    - 创建子进程失败之后，向父进程返回-1，并且设置errno

    父子进程之间的关系
    区别：
    - fork()函数的返回值不同
    - pcb中的一些数据，进程id，信号集等等

    共同点（子进程没有写操作时）
    - 用户区的数据共享
    - 文件描述符表

    父子进程之间的变量共享问题
    - 读时共享（除了内核区）
    - 写时拷贝（子进程需要对非内核区的变量进行写操作时）
*/


#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>

/*
    argc和argv都是用来处理命令行的参数
    argc: 是一个整数，表示传递给程序的命令行参数数量
    argv: 字符指针数组，包含了所有命令行参数的值，每一个元素都是指向字符串首地址的指针
*/
int main(int argc, char* argv[]) {
    int num = 10;
    pid_t pid = fork();
    // 判断父子进程
    if (pid > 0) {
        printf("pid_t pid = %d\n", pid);
        printf("parent process, pid = %d, ppid = %d\n", getpid(), getppid());
        num += 10;
        printf("parent num = %d\n", num);
    }
    else if (pid == 0) {
        printf("child process, pid = %d, ppid = %d\n", getpid(), getppid());
        num += 100;
        printf("child num = %d\n", num);
    }
    else {
        printf("create child process failure....\n");
    }

    //for循环
    for (int i = 0;i < 5;++i) {
        printf("pid = %d, i = %d\n", getpid(), i);
        sleep(1);
    }
    return 0;
}