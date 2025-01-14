/*
    创建fifo文件（有名管道）
    - 通过命令：mkfifo 有名管道名字
    - 通过函数：

    #include <sys/types.h>
    #include <sys/stat.h>

    int mkfifo(const char *pathname, mode_t mode);
    函数功能：创建管道

    参数
    - const char* pathname：管道名称的路径
    - mode_t mode：文件的权限，和 open 的 mode 是一样的
    - 在系统调用中，mode 会和 ~umask 做 & 操作

    返回值
    - 0：有名管道创建成功
    - -1：有名管道创建失败，并且生成系统调用的错误信息


*/
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char* argv[]) {

    //通过访问文件的方式，判断文件是否存在，存在（访问成功），返回0，不存在（访问失败），返回-1
    int ret = access("fifo1", F_OK);

    if (ret == -1) {
        // 文件访问失败
        int is_create = mkfifo("fifo1", 0664);
        if (is_create == -1) {
            perror("mkfifo");
            exit(0);
        }
        perror("access");
    }
    else {
        //文件访问成功
        perror("access");
        exit(0);
    }
    return 0;
}

