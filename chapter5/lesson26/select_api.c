/*
    I/O 多路转接技术：select() 系统调用介绍

    基本思路
    - 首先要构造一个关于文件描述符的列表，将要监听的文件描述符添加到改列表中。
    - 调用系统函数 select，监听该列表中的文件描述符，直到这些文件描述符中的一个或者多个进行 I/O
    操作时，该函数才返回
        - 这个函数是阻塞的
        - 函数对文件描述符的读缓冲区，检测操作是由内核完成的
    - select() 系统调用返回时，它会告诉进程有多少（哪些）描述符要进行 I/O操作


    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>

    int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
    函数功能：监听多个文件描述符，直到这些文件描述符中有一个或者多个可以进行 I/O 操作时，该函数才退出阻塞状态，并且返回
    函数参数： sizeof(fd_set) = 128 bytes = 1024 bits, 可以存储 1024 个文件描述符缓冲区的状态，当文件描述符读缓冲区
    可以操作时，对应的 readfds 集合中的 bit 位值是 1
    - nfds：委托内核检测的最大文件描述符的值 + 1（文件描述符个数），因为文件描述符的值可能是 0
    - readfds：要检测读缓冲区的文件描述符集合，一旦这些文件描述符集合的读缓冲区有数据，解除阻塞
    - writefds：要检测写缓冲区的文件描述符集合，一旦这些文件描述符集合的写缓冲区可以写数据，解除阻塞
    - exceptfds：要检测发生异常的文件描述符集合
    - timeout：设置 select() 函数阻塞的时间
        struct timeval {
            long    tv_sec;
            long    tv_usec;
        };
        - NULL：select() 函数永久阻塞，直到检测到了文件描述符有变化
        - tv_sec = 0 tv_usec = 0，不阻塞
        - tv_sec > 0 tv_usec > 0，阻塞对应的时间

    返回值
    - 成功：> 0，表示检测的 readfds、writefds 和 exceptfds 集合中，发生变化的文件描述符个数（一个文件描述符，对应一个 bit 位）
    - 失败：-1，并且设置错误号


    void FD_CLR(int fd, fd_set *set);
    函数功能：将指定文件描述符，所在文件描述符集合的标志位置 0
    函数参数
    - fd：指定的文件描述符
    - set：指定的文件描述符集合

    int  FD_ISSET(int fd, fd_set *set);
    函数功能： 判断文件描述符，所在文件描述符集合对应的位置，标志位的 bit 值
    函数参数
    - fd：指定的文件描述符
    - set：指定的文件描述符集合
    返回值：0 或 1，表示 bit 位上的值

    void FD_SET(int fd, fd_set *set);
    函数功能：将指定文件描述符，所在文件描述符集合的标志位置 1
    函数参数
    - fd：指定的文件描述符
    - set：指定的文件描述符集合

    void FD_ZERO(fd_set *set);
    函数功能：将文件描述符集合 set 中的所有 bit 位，初始化为 0
    函数参数
    - set：需要初始化的文件描述符集合

*/