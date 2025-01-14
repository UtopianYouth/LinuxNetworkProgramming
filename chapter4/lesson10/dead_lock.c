/*
    死锁常见的三种情况
    - 忘记释放锁
    - 重复加锁
    - 多线程多锁，抢占锁资源

    - 前两种比较好理解，当多线程要访问的临界资源只有一个的时候，也会出现
    - 第三种情况，出现在访问多个临界资源的时候

*/
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>

// 全局变量（存放在静态存储区）
int tickets = 500;

// 创建互斥量
pthread_mutex_t mutex;

void* sell_tickets(void* arg);

int main(int argc, char* argv[]) {

    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);

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

    // 释放互斥量
    pthread_mutex_destroy(&mutex);

    return 0;
}

void* sell_tickets(void* arg) {


    while (1) {

        // 对临界资源的访问加锁
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex);

        if (tickets > 0) {
            printf("thread id = %ld, selling tickets num is %d.\n", pthread_self(), 500 - tickets + 1);
            --tickets;
        }
        else {
            // 访问完毕解锁
            pthread_mutex_unlock(&mutex);
            break;
        }

        // 访问完毕解锁
        pthread_mutex_unlock(&mutex);

        // microseconds
        usleep(6000);
    }

    return NULL;
}