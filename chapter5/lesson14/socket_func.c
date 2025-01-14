/*
    #include <sys/types.h>
    #include <sys/socket.h>

    int socket(int domain, int type, int protocol);
    函数功能：创建一个套接字，用于网络通信
    函数参数
    - domain：TPC/IP 协议族（地址族），常见的AF_INET(IPv4), AF_INET6(IPv6), AF_LOCAL(local communication)
    - type：通信过程中使用的协议类型
        - SOCK_STREAM：提供一个有序的、可靠的、双向通信的、基于连接的字节序协议类型（TCP）
        - SOCK_DGRAM：提供一个面向无连接的、不可靠的、任意数据长度的数据报式（datagrams）协议类型（UDP）
        ...
    - protocol：使用具体的协议，一般写 0 即可
        - SOCK_STREAM：流式协议默认使用 TCP
        - SOCK_DGRAM：报式协议默认使用 UDP
    返回值
    - 成功：返回一个该 socket 的文件描述符
    - 失败：返回 -1，并且设置错误号，具体参考 linux 源码说明，man 2 socket

    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    函数功能：将创建好的 socket 文件描述符（fd）和本地的 IP 地址 + 端口进行绑定（也叫 socket 命名）
    函数参数
    - sockfd：通过 socket() 函数得到的文件描述符
    - addr：需要绑定的 socket 地址，这个地址封装了 ip 和端口号的信息
    - addrlen：指定 addr 所指向的 socket 结构体大小
    返回值
    - 成功：返回 0
    - 失败：返回-1，并且设置错误号，具体参考 linux 源码说明，man 2 bind

    int listen(int sockfd, int backlog);
    函数功能：监听 sockfd 绑定的 socket 上的连接
    函数参数
    - sockfd：通过 socket() 函数得到的文件描述符
    - backlog：监听队列的最大长度，即在调用 accept 之前可以排队未完成连接请求的数量，pending connection：待连接
        - 一般的，使用 /proc/sys/net/core/somaxconn 查看系统支持的最大监听队列长度
    返回值
    - 成功：返回 0
    - 失败：返回-1，并且设置错误号，具体参考 linux 源码说明，man 2 listen

    int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    函数功能：接收客户端发来的网络通信连接请求，该函数会阻塞，等待客户端连接
    函数参数
    - sockfd：用于监听的文件描述符
    - addr：socket 地址，传出参数，记录了连接成功后，客户端的地址信息（ip，port）
    - addrlen：第二个参数结构体所占内存的大小
    返回值
    - 成功：用于通信的文件描述符（每一个客户端请求连接，连接成功后，都会返回一个文件描述符）
    - 失败：返回 -1，并且设置错误号，具体参考 linux 源码说明，man 2 accept

    int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    函数功能：客户端连接服务器
    函数参数
    - sockfd：用于与客户端通信的文件描述符
    - addr：客户端要连接的服务器地址信息
    - addrlen：第二个参数所占内存的大小
    返回值
    - 成功：返回 0
    - 失败：返回 -1，并且设置错误号，具体参考 linux 源码说明，man 2 connect


    网络通信基础方式，向缓冲区中读写数据
    #include <unistd.h>

    ssize_t write(int fd, const void *buf, size_t count);
    函数功能：向缓冲区写数据
    函数参数
    - fd：需要写数据缓冲区的文件描述符
    - buf：缓冲区所在内存的地址
    - count：写入数据的字节数
    返回值
    - 成功：返回成功写入数据的字节数
    - 失败：返回 -1，并且设置错误号，具体参考 linux 源码说明，man 2 write

    ssize_t read(int fd, void *buf, size_t count);
    函数功能：从缓冲区读数据
    函数参数
    - fd：读缓冲区的文件描述符
    - buf：缓冲区所在内存的地址
    - count：读数据的字节数
    返回值
    - 成功：返回成功读取数据的字节数
    - 失败：返回 -1，并且设置错误号，具体参考 linux 源码说明，man 2 read

    sockaddr结构体
    struct sockaddr {
        sa_family_t sa_family;
        char        sa_data[14];
    }

*/