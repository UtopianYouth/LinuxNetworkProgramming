/*
    读写锁的特点：主要解决互斥量，在读共享资源的时候，多线程不能同时进行
    - 如果有其他线程读数据，则允许其他线程进行读操作，但不能进行写操作
    - 如果有其他线程写数据，其他线程都不允许读和写操作
    - 写是独占的（原子性，不允许中断），写的优先级高

    #include <pthread.h>

    读写锁类型：pthread_rwlock_t

    int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
        const pthread_rwlockattr_t *restrict attr);

    int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);

    int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);

    int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);

    int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);

    int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);

    int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

    案例：创建八个线程，操作同一个全局变量
    - 3个线程写全局变量
    - 5个线程读全局变量

*/

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>


// 全局变量（共享资源）
int global_variable = 100;

// 定义读写锁
pthread_rwlock_t rwlock;

// 读全局变量子线程
void* read_global(void* arg) {

    sleep(1);

    // 获得读写锁的读锁，阻塞写共享资源的线程
    pthread_rwlock_rdlock(&rwlock);

    printf("read global variable = %d.\n", global_variable);

    pthread_rwlock_unlock(&rwlock);

    return NULL;
}

// 写全局变量子线程
void* write_global(void* arg) {

    // 获得读写锁的写锁，阻塞读写共享资源的线程
    pthread_rwlock_wrlock(&rwlock);

    ++global_variable;
    printf("write global variable = %d.\n", global_variable);

    pthread_rwlock_unlock(&rwlock);

    return NULL;
}

int main(int argc, char* argv[]) {
    // 初始化读写锁
    pthread_rwlock_init(&rwlock, NULL);

    // 初始化线程属性
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // 设置线程分离
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // 创建三个读全局变量子线程
    for (int i = 0;i < 3;++i) {
        pthread_t tid;
        pthread_create(&tid, &attr, read_global, NULL);
    }

    // 创建五个写全局变量子线程
    for (int i = 0;i < 5;++i) {
        pthread_t tid;
        pthread_create(&tid, &attr, write_global, NULL);
    }

    // 主线程退出
    pthread_exit(NULL);

    // 释放读写锁资源
    pthread_rwlock_destroy(&rwlock);

    return 0;
}