/*
    #include <sys/epoll.h>

    int epoll_create(int size);
    函数功能：在内核中创建一个新的 epoll 实例，该 epoll 实例中有两个比较重要的数据
    - 一个是需要检测的文件描述符集合（红黑树存储）
    - 一个是就绪列表，存放缓冲区数据发生改变的文件描述符集合（双向链表）
    函数参数：
    - size：没有实际意义，传递一个大于 0 的值即可
    返回值
    - 成功：返回大于零的值，是一个文件描述符
    - 失败：返回 -1 并且设置错误号
    typedef union epoll_data {
        void        *ptr;
        int          fd;
        uint32_t     u32;
        uint64_t     u64;
    } epoll_data_t;

    struct epoll_event {
        uint32_t     events;
        epoll_data_t data;
    };

    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
    函数功能：对 epoll_create() 创建的实例进行管理，向文件描述符集合中，添加文件描述符信息、删除信息、修改信息
    函数参数
    - epfd：epoll 实例对应的文件描述符
    - op：对 epoll 实例存储的文件描述符集合进行什么样的操作
        - EPOLL_CTL_ADD：添加文件描述符
        - EPOLL_CTL_MOD：修改文件描述符
        - EPOLL_CTL_DEL：删除文件描述符
    - fd：要进行操作的文件描述符
    - event：绑定操作文件描述符所发生的事件
    返回值
    - 成功：0
    - 失败：-1，并且设置相关错误号

    int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
    函数功能：检测 epoll 实例的文件描述符集合中，有多少文件描述符对应的缓冲区发生了变化
    函数参数
    - epfd：epoll 实例对应的文件描述符
    - events：传出参数，保存发生了变化的文件描述符信息
    - maxevents：第二个参数 events 结构体数组的大小
    - timeout：阻塞时间
        - 0：不阻塞
        - -1：阻塞，直到检测到文件描述符数据发生变化，解除阻塞
        - >0：阻塞的时长（microsecond）
    返回值
    - 成功：返回缓冲区发生变化的文件描述符个数
    - 失败：返回 -1 并且设置错误号
*/