/*
    使用多线程实现卖票案例
    有 3 个窗口，一共 20 张票
    - 创建三个子线程
    - 回收子线程资源（也可以设置线程分离）
    - 主线程退出

*/

#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>

// 全局变量（存放在静态存储区）
int tickets = 20;

void* sell_tickets(void* arg) {
    while (tickets > 0) {
        printf("thread id = %ld, selling tickets num is %d.\n", pthread_self(), 20 - tickets + 1);
        --tickets;
        // microseconds
        sleep(1);
    }
    return NULL;
}

int main(int argc, char* argv[]) {

    for (int i = 0;i < 3;++i) {
        pthread_t tid;

        // 创建线程属性
        pthread_attr_t attr;

        // 初始化和设置线程分离状态
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        // 创建线程
        pthread_create(&tid, &attr, sell_tickets, NULL);

        // 销毁线程属性
        pthread_attr_destroy(&attr);

    }

    // 主线程退出
    pthread_exit(NULL);
    return 0;
}