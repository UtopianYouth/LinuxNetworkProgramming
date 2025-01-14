/*
        man 3 execlp 查看源码

        #include <unistd.h>
        int execlp(const char *file, const char *arg, ...);


        参数解读
        - file: 需要执行可执行文件的文件名（与execl的区别是，不需要指定可执行文件的绝对路径，自动在系统的环境变量中寻找）
        - arg: 可执行文件所需要的参数列表，后面省略号代表可变参数
            - 第一个参数可以理解为参数列表集合的名称，没什么作用，为了方便，一般写可执行程序的名称
            - 从第二个参数开始往后，就是程序执行所需要的参数列表
            - 参数最后需要以NULL结束（哨兵）

        返回值
        - 调用成功，没有返回值（很好理解，进程的虚拟地址空间，除了内核区，都被调用的可执行文件覆盖了，找不到现场，无法返回）
        - 调用失败，返回值为-1，并且设置errno
*/

#include <unistd.h>
#include<stdio.h> 

int main(int argc, char* argv[]) {
    size_t pid = fork();
    if (pid > 0) {
        // 父进程
        printf("parent process, pid = %d\n", getpid());
    }
    else if (pid == 0) {
        printf("child process, pid = %d\n", getpid());
        // 子进程执行execlp函数，对应进程虚拟地址空间，除了内核区，全部被可执行文件覆盖
        execlp("ps", "ps", "aux", NULL);
    }

    for (int i = 0;i < 3;++i) {
        printf("pid = %d, i = %d\n", getpid(), i);
    }
    return 0;
}