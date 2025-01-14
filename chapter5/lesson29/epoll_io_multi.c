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
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/epoll.h>

int main(int argc, char* agrv[]) {
    // 创建 socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("listen");
        exit(-1);
    }

    // 绑定 ip + port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9999);
    int ret1 = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret1 == -1) {
        perror("bind");
        exit(-1);
    }
    // 监听
    listen(listen_fd, 128);

    // 创建一个 epoll 实例
    int epoll_fd = epoll_create(100);
    if (epoll_fd == -1) {
        perror("epoll_fd");
        exit(-1);
    }

    // 将 listen_fd 加入到 epoll 实例中
    struct epoll_event listen_epev;    // 绑定需要检测的事件
    listen_epev.events = EPOLLIN;
    listen_epev.data.fd = listen_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &listen_epev);

    // 存储检测到发生变化的文件描述符
    struct epoll_event epevs[1024];
    while (1) {
        int ret2 = epoll_wait(epoll_fd, epevs, 1024, -1);
        if (ret2 == -1) {
            perror("epoll_wait");
            exit(-1);
        }
        printf("ret2 = %d\n", ret2);

        for (int i = 0;i < ret2;++i) {
            if (epevs[i].data.fd == listen_fd) {
                // 有新的客户端连接进来
                struct sockaddr_in client_addr;
                socklen_t addr_len = sizeof(client_addr);
                int communication_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &addr_len);
                if (communication_fd == -1) {
                    perror("accept");
                    exit(-1);
                }
                printf("connect success.\n");

                char client_ip[16] = { '\0' };
                unsigned short client_port = ntohs(client_addr.sin_port);
                inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
                printf("client ip = %s, client_port = %u.\n", client_ip, client_port);

                // 将新客户端的通信文件描述符加入到 epoll 实例中
                struct epoll_event communication_epev;
                communication_epev.data.fd = communication_fd;
                communication_epev.events = EPOLLIN;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, communication_fd, &communication_epev);
            }
            else {
                // 没有新的客户端连接，进行通信，检测读事件
                if (epevs[i].events & EPOLLIN != EPOLLIN) {
                    continue;
                }
                char receive_buf[1024] = { '\0' };
                int ret3 = read(epevs[i].data.fd, receive_buf, sizeof(receive_buf));
                if (ret3 == -1) {
                    perror("read");
                    exit(-1);
                }
                else if (ret3 == 0) {
                    printf("client closed.\n");
                    // 删除 epoll 实例里面的文件描述符信息
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, epevs[i].data.fd, NULL);
                    close(epevs[i].data.fd);
                }
                else {
                    printf("receive data: %s\n", receive_buf);
                    write(epevs[i].data.fd, receive_buf, sizeof(receive_buf));
                }
            }
        }
    }

    // 关闭相应的文件描述符
    close(epoll_fd);
    close(listen_fd);
    return 0;
}