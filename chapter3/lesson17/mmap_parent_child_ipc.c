/*
        #include <sys/mman.h>

        void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
        - 功能：将一个文件或者设备的数据映射到内存空间中
        - 参数
            - void* addr: 创建文件或设备内存映射的地址，一般程序员不好指定，赋值为NULL，由内核决定
            - size_t length: 要映射数据的长度（一般为页的整数倍，大于等于文件长度）
                - 获取文件的长度API: stat(), lseek()
            - int prot: 内存映射保护的权限（操作的权限，不能和打开的文件权限有冲突）
                - PROT_NONE: 没有权限
                - PROT_EXEC: 可执行权限
                - PROT_WRITE: 写权限
                - PROT_READ: 读权限
            - int flags: 确定是否将内存映射更新的内容覆盖到打开的文件中
                - MAP_SHARED: 内存映射更新的内容会覆盖打开的文件，和打开的文件同步，如果要实现ipc，使用该参数
                - MAP_PRIVATE: 内存映射更新的内容不会覆盖打开的文件，会重新创建一个新的文件
                （copy on write，和子进程用户区一样，读时共享，写时拷贝）
            - int fd: 进行内存映射的文件描述符，通过 open 系统调用得到打开文件的描述符
                - 注意：文件大小必须大于0，open 时和 prot 参数权限不能有冲突，应该要高于 prot 的权限
            - offset: 内存映射的偏移量，必须是逻辑页大小的整数倍，一般使用0，不需要偏移

        - 返回值
            - 成功：返回内存映射区域的首地址
            - 失败：返回MAP_FAILED, (void *)(-1), 并且设置系统调用错误信息

        int munmap(void *addr, size_t length);
        - 功能：释放内存映射
        - 参数
            - void* addr: 需要释放内存映射的首地址（这个地址一般根据 mmap 的返回值得到）
            - size_t length: 需要释放内存映射的长度，和 mmap() 中的 length 一致
        - 返回值
            - 释放成功：0
            - 释放失败：-1

*/

#include<stdio.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>


/*
    内存映射实现进程间通信
*/
int main() {
    // 1. 打开文件
    int fd = open("./test_mmap.txt", O_RDWR);
    if (fd == -1) {
        printf("open file failure...\n");
        exit(0);
    }

    // 2. 创建内存映射区
    int length = lseek(fd, 0, SEEK_END);
    printf("length = %d\n", length);
    void* ptr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);


    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    // 3. 创建子进程
    pid_t pid = fork();

    // 4. 父进程读取内存映射区数据，子进程向内存映射区写数据
    if (pid > 0) {
        int status_code = wait(NULL);     //等待子进程运行完毕，回收子进程，参数获取子进程退出时的状态码
        char buf_read[1024] = { '\0' };
        strcpy(buf_read, (char*)ptr);
        printf("read data: %s\nchild exit status_code: %d\n", buf_read, status_code);
    }
    else if (pid == 0) {
        strcpy((char*)ptr, "hello test...");
    }
    else {
        printf("open child process failure...\n");
        exit(0);
    }

    // 5. 关闭内存映射区
    if (pid > 0) {
        close(fd);
        munmap(ptr, length);
        getchar();
    }

    return 0;
}