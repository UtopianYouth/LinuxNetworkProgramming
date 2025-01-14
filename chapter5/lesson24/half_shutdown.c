/*
    半关闭：从程序的角度来看，可以使用 API 来控制实现半连接状态

    #include <sys/socket.h>
    int shutdown(int sockfd, int how);
    函数功能：关闭文件描述符 sockfd 绑定的 socket 对应的全双工通信连接
    函数参数
    - sockfd：需要关闭的 socket 的 fd
    - how：允许为 shutdown 操作选择以下几种方式
        - SHUT_RD: 对应 0，关闭 sockfd 上的读功能，此选项不允许 sockfd 进行读操作，并且，该套接字不再接收数据。
        任何当前在套接字，接收缓冲区的数据将被无声的丢弃掉（sockfd 是缓冲区对应的文件描述符，套接字在缓冲区中操作数据）
        - SHUT_WR: 对应 1，关闭 sockfd 的写功能，此选项不允许 sockfd 进行写操作，进程不能在对此套接字发出写操作
        - SHUT_RDWR: 对应2，关闭 sockfd 的读写功能，相当于调用 shutdown 两次，首先是 SHUT_RD，然后是 SHUT_WR
    返回值
    - 成功：0
    - 失败：-1，并且设置错误号，具体参考 man 2 shutdown


    socket 通信，从缓冲区中接收数据 API
    #include <sys/types.h>
    #include <sys/socket.h>

    ssize_t recv(int sockfd, void *buf, size_t len, int flags);

    ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);

    ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);

*/