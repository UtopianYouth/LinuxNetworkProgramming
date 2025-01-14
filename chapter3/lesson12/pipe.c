/*
    #include <unistd.h>

    int pipe(int pipefd[2]);

    函数功能：创建一个匿名管道，用来进程间通信

    参数
    int pipefd[2]: 传出参数
    - pipefd[0]: 对应管道的读端
    - pipefd[1]: 对应管的道写端

    返回值
    - 成功创建管道，返回 0
    - 失败创建管道，返回 -1

    tip：匿名管道只能用于亲子进程之间的通信，因为亲子进程在创建的过程中，具有相同的内核区文件描述符（父子进程，兄弟进程）
*/

//匿名管道实例，子进程向管道写数据，父进程从管道读数据（可以相互写）
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(int argc, char* argv[]) {
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");     //打印pipe系统调用失败的错误信息
        exit(0);
    }
    pid_t pid = fork();
    if (pid > 0) {
        char buf_read[1024] = { 0 };
        // char buf_write[] = "parent to child";
        while (1) {
            //父进程从管道的读端读数据
            read(pipefd[0], buf_read, sizeof(buf_read));
            printf("parent process receive: %s, pid = %d\n", buf_read, getpid());
            //读完数据打印后，重置
            bzero(buf_read, 1024);

            // //父进程从管道的写端写数据
            // write(pipefd[1], buf_write, sizeof(buf_write));
            // printf("parent process write to child: %s, pid = %d\n", buf_write, getpid());
            // sleep(1);
        }
    }
    else if (pid == 0) {
        // char buf_read[1024] = { 0 };
        char buf_write[] = "child to parent";
        while (1) {
            //子进程从管道的写端写数据
            write(pipefd[1], buf_write, sizeof(buf_write));
            printf("child process write to parent: %s, pid = %d\n", buf_write, getpid());
            sleep(1);

            // //子进程从管道的读端读数据
            // read(pipefd[0], buf_read, sizeof(buf_read));
            // printf("child process receive: %s, pid = %d\n", buf_read, getpid());
            // //读完数据打印后，重置
            // bzero(buf_read, 1024);
        }
    }
    else {
        printf("create process failure....\n");
    }

    return 0;
}