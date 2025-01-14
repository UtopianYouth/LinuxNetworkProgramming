#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

/*
    向有名管道中写数据
    - 判断有名管道是否存在，不存在，要先创建有名管道
    - 以只写的方式打开有名管道
    - 向有名管道中写入数据
    - 关闭有名管道（文件）


*/

int main(int argc, char* argv[]) {

    // 判断有名管道文件是否存在
    int ret = access("./test", F_OK);

    if (ret == -1) {
        // 文件访问失败
        int is_create = mkfifo("./test", 0664);
        if (is_create == -1) {
            perror("mkfifo");
        }
        perror("access");
    }

    // 只写方式打开有名管道（其实就是打开文件）
    int fd = open("./test", O_WRONLY);

    // 打开管道文件失败
    if (fd == -1) {
        perror("open");
        exit(0);
    }

    // 写入数据
    for (int i = 0;i < 100;++i) {
        char buf_write[1024] = { '\0' };
        //向缓冲区里面写数据
        sprintf(buf_write, "hello: %d\n", i);

        printf("write named piped: %s", buf_write);

        //向有名管道写入数据
        write(fd, buf_write, sizeof(buf_write));

        //重置缓冲区内容
        memset(buf_write, 0, sizeof(buf_write));
        sleep(1);
    }

    //关闭有名管道（文件）
    close(fd);

    return 0;
}