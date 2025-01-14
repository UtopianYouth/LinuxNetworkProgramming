#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>

/*
    #include<stdlib.h>
    void exit(int status);
    该函数执行三步
    进程运行
    - 调用进程退出处理函数
    - 刷新I/O缓冲，关闭文件描述符
    - 调用_exit()系统调用
    进程终止运行

    #include<unistd.h>
    void _exit(int status);
    该函数执行一步
    进程运行
    - 调用_exit()系统调用
    进程终止运行


    status参数: 是进程退出时的一个状态信息，父进程回收子进程资源的时候可以获取到
*/

int main(int argc, char* argv[]) {
    printf("hello\n");      // ‘\n’ 会自动刷新I/O缓冲区，输出到控制台
    printf("world");

    //exit(0);                // 刷新I/O缓冲区，world会输出到控制台中，status = 0 返回给父进程

    _exit(0);               // 不会刷新I/O缓冲区，world不会输出
    return 0;
}