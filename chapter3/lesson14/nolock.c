#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>

/*
    设置管道非阻塞
    int flag = fcntl(fd[0], F_GETFL);   // 获取原来的flag
    flag |= O_NONBLOCK;                // 修改flag的值
    fcntl(fd[0], F_SETFL, flag);       // 设置新的flag
*/

int main(int agrc, char* argv[]) {
    // fork之前创建管道
    int pipefd[2];
    int ret = pipe(pipefd);

    if (ret == -1) {
        perror("pipe");     //打印系统调用的错误
        exit(0);
    }

    pid_t pid = fork();

    if (pid > 0) {
        //设置管道非阻塞
        int flag = fcntl(pipefd[0], F_GETFL);   // 获取原来的flag
        flag |= O_NONBLOCK;                     // 按位或操作，修改flag的值
        fcntl(pipefd[0], F_SETFL, flag);        // 设置新的flag

        //父进程从管道的读端读数据
        char buf_read[1024] = { 0 };
        int len = 0;
        while (1) {
            // 关闭写端
            close(pipefd[1]);

            len = read(pipefd[0], buf_read, sizeof(buf_read));
            printf("parent process read pipe, pid = %d\n", getpid());
            printf("len = %d %s\n", len, buf_read);
            memset(buf_read, 0, sizeof(buf_read));
            sleep(5);
        }
    }
    else if (pid == 0) {
        // 设置管道非阻塞
        int flag = fcntl(pipefd[1], F_GETFL);   // 获取原来的flag
        flag |= O_NONBLOCK;                     // 按位或操作，修改flag的值
        fcntl(pipefd[1], F_SETFL, flag);        // 设置新的flag


        //子进程从管道的写端写数据
        char buf_write[] = "hello pipe.";
        int len = 0;
        while (1) {
            // 关闭读端
            close(pipefd[0]);

            len = write(pipefd[1], buf_write, sizeof(buf_write));
            printf("child process write pipe, pid = %d\n", getpid());
            printf("len = %d %s\n", len, buf_write);
            sleep(1);
        }
    }
    else {
        printf("create child process failure...\n");
    }

    return 0;
}