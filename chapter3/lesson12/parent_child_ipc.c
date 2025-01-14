#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<wait.h>
#include<string.h>

/*
    实现 ps aux | grep xxx  父子进程间通信
    子进程：ps aux, 子进程结束后, 将数据经过管道发送给父进程
    父进程：从管道获取数据
    pipe()
    execlp()
    子进程将标准输出 stdout_fileno 重定向到管道的写端, dup2
*/


int main(int argc, char* argv[]) {
    // 创建管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        exit(0);
    }
    // 创建子进程
    pid_t pid = fork();
    if (pid > 0) {
        // 父进程
        // 关闭写端
        close(pipefd[1]);

        // 从管道中读数据
        char buf_read[1024] = { 0 };
        int len = -1;
        printf("parent process receive:\n");
        while (len = read(pipefd[0], buf_read, sizeof(buf_read) - 1) > 0) {
            printf("%s", buf_read);     //ps aux产生的内容存在 '\n'  
            memset(buf_read, 0, sizeof(buf_read));      //清空字符串里面的内容
        }
        wait(NULL);
    }
    else if (pid == 0) {
        // 子进程
        // 关闭读端
        close(pipefd[0]);

        // 文件描述符重定向，将终端打印的字符重定向到管道的写端，stdout_fileno -> fd[1]
        dup2(pipefd[1], STDOUT_FILENO);

        //执行 ps aux
        execlp("ps", "ps", "aux", NULL);
        perror("execlp");       //打印系统调用API执行失败错误

        //子进程退出
        exit(0);
    }
    else {
        printf("create process failure....\n");
    }

    return 0;
}