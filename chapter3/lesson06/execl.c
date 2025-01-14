/*
        man 3 execl 查看源码

        #include <unistd.h>
        int execl(const char *path, const char *arg, ...);

        参数解读
        - path: 需要指定执行文件的路径或者名称，推荐使用绝对路径，如/home/xxxx/xxx/x.out
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
        // 子进程执行execl函数，对应进程虚拟地址空间，除了内核区，全部被可执行文件覆盖
        // execl除了执行自定义的可执行程序，还可以执行linux OS中的可执行程序
        // execl("/home/utopiayouth/Desktop/chapter3/lesson6/hello", "hello", NULL);
        execl("/bin/ps", "ps", "aux", NULL);
        perror("execl");
    }

    for (int i = 0;i < 3;++i) {
        printf("pid = %d, i = %d\n", getpid(), i);
    }
    return 0;
}