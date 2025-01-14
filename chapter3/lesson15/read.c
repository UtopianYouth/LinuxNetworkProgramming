#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

/*
    从管道中读取数据
    - 打开有名管道
    - 从有名管道中读取数据
    - 关闭有名管道
*/


int main(int argc, char* argv[]) {

    // 打开管道文件
    int fd = open("./test", O_RDONLY);

    if (fd == -1) {
        perror("open");
        exit(0);
    }

    //从有名管道中读数据
    while (1) {
        int len = 0;
        char buf_read[1024] = { '\0' };
        len = read(fd, buf_read, sizeof(buf_read));
        if (len <= 0) {
            // 有名管道数据读取完毕
            perror("read");
            break;
        }
        printf("read named piped: %s", buf_read);
        //重置缓冲区内容
        memset(buf_read, 0, sizeof(buf_read));
    }

    // 关闭有名管道
    close(fd);
    return 0;
}