#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>

/*
    获取匿名管道的大小
*/


int main(int argc, char* argv[]) {
    int pipefd[2];
    int ret = pipe(pipefd);

    long pipe_write_size = fpathconf(pipefd[0], _PC_PIPE_BUF);
    printf("pipe_read size = %ld bytes\n", pipe_write_size);
    return 0;
}