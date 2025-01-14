/*
    端口复用
    - 防止服务器重启时，之前绑定的端口还未释放（TCP连接释放，处于 TIME_WAIT 阶段）
    - 程序突然退出而系统没有释放端口（也是 TIME_WAIT 阶段）

    #include <sys/types.h>
    #include <sys/socket.h>

    int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
    函数功能：获取套接字选项的系统调用，查询与指定套接字相关的各种配置参数
    函数参数
    - sockfd：需要查询套接字的fd
    - level：选项所在的协议级别，通常为 SOL_SOCKET（端口复用级别）
    - optname：要查询具体选项的名称
        - SO_RCVBUF（接收缓冲区大小）
        - SO_SNDBUF（发送缓冲区大小）
    - optval：optname 取值所在的地址（optname，有些取值是 int 类型，有些是特定的结构体类型）
    - optlen：optval 缓冲区长度
    返回值
    - 成功：返回 0
    - 失败：返回 -1，并且设置错误号

    int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
    函数功能：设置套接字属性（不仅仅能设置端口复用）
    函数参数
    - sockfd：需要操作 socket 的文件描述符
    - level：所设置套接字属性的协议及别，和 getsockopt() 一样，通常为 SOL_SOCKET（端口复用级别）
    - optname：需要设置选项的名称
        - SO_REUSEADDR：地址复用
        - SO_REUSEPORT：端口复用
    - optval：optname 取值所在的地址（optname，有些取值是 int 类型，有些是特定的结构体类型）
        - 当 optname 是 SO_REUSEPORT时
            - *optval = 1：表示可以端口复用
            - *optval = 0：表示不可以端口复用
    - optlen：optval 参数的大小
    返回值
    - 成功：0
    - 失败：-1，并且设置错误号

    端口复用，设置的时机是在服务器绑定端口之前
    setsockopt();
    bind();

*/