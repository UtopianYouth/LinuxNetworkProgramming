/*
    I/O 多路复用技术：poll() 系统调用

    #include <poll.h>
    int poll(struct pollfd *fds, nfds_t nfds, int timeout);
    函数功能：
    函数参数
    - fds：结构体数组指针，需要检测文件描述符的集合
    - nfds：第一个参数数组中，最后一个有效元素的下标 + 1，即表示委托内核需要遍历文件描述符边界
    - timeout：poll() 函数的阻塞时长
        - 0：不阻塞
        - (-1)：阻塞，当检测到需要检测的文件描述符有变化，解除阻塞
        - (>0)：阻塞时长，million second，毫秒
    返回值
    - 失败 ：-1
    - 成功：大于 0，返回检测到的集合中，有多少个文件描述符的 revents 值是非 0，等于 0，表示在 poll() 的阻塞时间内，没有文件描述符可以操作

    struct pollfd {
        int   fd;           // 委托内核需要检测的文件描述符
        short events;       // 委托内核检测文件描述符的什么事件
        short revents;      // 文件描述符实际发生的事件
    };


*/