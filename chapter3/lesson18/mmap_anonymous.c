#include<stdio.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>

/*
    匿名内存映射（不依靠文件描述符的内存映射）
    - 创建匿名内存映射区
    - 父进程向匿名内存映射区写内容
    - 子进程从匿名内存映射区读内容
    - 释放内存映射资源
*/
int main(int argc, char* argv[]) {
    int len = 4096;

    // flags 参数种省去了MAP_ANONYMOUS
    void* ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, -1, 0);

    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    pid_t pid = fork();
    if (pid > 0) {
        strcpy((char*)ptr, "hello world...");
        wait(NULL);
    }
    if (pid == 0) {
        sleep(1);
        char buf[4096] = { '\0' };
        strcpy(buf, (char*)ptr);
        printf("%s", buf);
    }

    if (pid > 0) {
        int status = munmap(ptr, len);
        if (status == -1) {
            perror("munmap");
            exit(0);
        }
    }
    return 0;
}