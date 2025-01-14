#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>


// 创建两个共享资源
int share1 = 1;
int share2 = 2;

// 创建两个互斥量
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;


// 子线程 A 逻辑
void* workA(void* arg) {
    // 假设该子线程先访问 share1，再访问 share2 共享资源
    pthread_mutex_lock(&mutex1);
    printf("share1 = %d\n", share1);
    sleep(1);   // 阻塞 1s ，防止同时获得两个锁

    pthread_mutex_lock(&mutex2);
    printf("share2 = %d\n", share2);

    // 解锁互斥量
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);

    // 子线程退出
    return NULL;
}

// 子线程 B 逻辑
void* workB(void* arg) {

    // 假设该子线程先访问 share2，再访问 share1 共享资源
    pthread_mutex_lock(&mutex2);
    printf("share2 = %d\n", share2);
    sleep(1);   // 阻塞 1s ，防止同时获得两个锁

    pthread_mutex_lock(&mutex1);
    printf("share1 = %d\n", share1);

    // 解锁互斥量
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);

    // 子线程退出
    return NULL;
}

int main(int argc, char* argv[]) {
    // 初始化互斥量
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    // 创建线程属性
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // 设置线程属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // 创建两个线程
    pthread_t tid1, tid2;
    pthread_create(&tid1, &attr, workA, NULL);
    pthread_create(&tid2, &attr, workB, NULL);

    // 释放线程属性
    pthread_attr_destroy(&attr);

    // 主线程退出
    pthread_exit(NULL);

    // 释放互斥量
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}