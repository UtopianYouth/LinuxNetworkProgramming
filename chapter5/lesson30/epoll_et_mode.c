#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<errno.h>

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

                // 设置 communication_fd 属性非阻塞
                int flag = fcntl(communication_fd, F_GETFL);
                flag | O_NONBLOCK;
                fcntl(communication_fd, F_SETFL, flag);


                char client_ip[16] = { '\0' };
                unsigned short client_port = ntohs(client_addr.sin_port);
                inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
                printf("client ip = %s, client_port = %u.\n", client_ip, client_port);

                // 将新客户端的通信文件描述符加入到 epoll 实例中
                struct epoll_event communication_epev;
                communication_epev.data.fd = communication_fd;
                communication_epev.events = EPOLLIN | EPOLLET;      // 设置边沿触发
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, communication_fd, &communication_epev);
            }
            else {
                // 没有新的客户端连接，进行通信，检测读事件
                if (epevs[i].events & EPOLLIN != EPOLLIN) {
                    continue;
                }

                // 边沿触发，一次性读取所有数据
                char receive_buf[5] = { '\0' };
                int len = 0;
                while ((len = read(epevs[i].data.fd, receive_buf, sizeof(receive_buf))) > 0) {
                    printf("receive data: %s\n", receive_buf);
                    write(epevs[i].data.fd, receive_buf, len);
                }

                if (len == 0) {
                    printf("client close\n");
                }
                else if (len == -1) {
                    if (errno == EAGAIN) {
                        // 删除 epoll 实例里面的文件描述符信息
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, epevs[i].data.fd, NULL);
                        close(epevs[i].data.fd);
                        printf("client closed.\n");
                    }
                    else {
                        perror("read");
                        exit(-1);
                    }
                }
            }
        }
    }

    // 关闭相应的文件描述符
    close(epoll_fd);
    close(listen_fd);
    return 0;
}