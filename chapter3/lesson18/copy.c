#include<stdio.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>

/*
    使用内存映射实现文件拷贝的功能
    - 打开原始文件
    - 创建一个新文件，并且拓展该文件（内存映射要求文件长度不能为0）
    - 分别进行内存映射
    - 内存拷贝
    - 释放资源（考虑到依赖关系，先申请的资源后释放）
*/
int main(int argc, char* argv[]) {

    // 打开原始文件
    int fd1 = open("./english.txt", O_RDWR);
    if (fd1 == -1) {
        perror("open file failure...");
        exit(0);
    }
    size_t length = lseek(fd1, 0, SEEK_END);

    // 创建一个新的文件，并且拓展该文件
    int fd2 = open("./copy.txt", O_RDWR | O_CREAT);
    if (fd2 == -1) {
        perror("create file failure...");
        exit(0);
    }
    truncate("./copy.txt", length);


    // 分别进行内存映射
    void* ptr1 = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
    void* ptr2 = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

    // 内存拷贝
    strcpy((char*)ptr2, (char*)ptr1);

    // 释放资源
    close(fd2);
    close(fd1);
    munmap(ptr1, length);
    munmap(ptr2, length);

    return 0;
}