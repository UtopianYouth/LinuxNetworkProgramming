/*
    #include<pthread.h>

    互斥量：顾名思义，就是同一时刻只能有一个线程访问的量

    互斥量类型：pthread_mutex_t

    int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);
    函数功能：初始化互斥量
    函数参数
    - mutex：需要初始化的互斥量对象
    - mutexattr：初始化的互斥量属性，一般默认使用NULL即可
    返回值
    - 只有 0，不管是成功还是失败
    TIPS
    - restrict：C语言的关键字（修饰符），被 restrict 修饰的指针所指向的内存，不能被另一个指针操作

    int pthread_mutex_destroy(pthread_mutex_t *mutex);
    函数功能：释放互斥量资源
    函数参数
    - mutex：需要释放的互斥量对象
    返回值
    - 成功：0
    - 失败：返回 EBUSY 错误号，说明当前互斥量已经被上锁，无法释放

    int pthread_mutex_lock(pthread_mutex_t *mutex);
    函数功能：加锁，会造成阻塞，如果有一个线程加锁了，那么其他线程只能阻塞等待
    函数参数
    - mutex：需要进行加锁操作的互斥量
    返回值
    - 成功：0
    - 失败
        - EINVAL：mutex 没有被初始化
        - EDEADLK：mutex 已经被其他线程进行了上锁

    int pthread_mutex_trylock(pthread_mutex_t *mutex);
    函数功能：尝试加锁，如果加锁失败，不会阻塞，会直接返回
    函数参数
    - mutex：尝试加锁的互斥量
    返回值
    - 成功：0
    - 失败
        - EINVAL：mutex 没有被初始化
        - EBUSY：mutex 当前已经被上锁，无法加锁
    int pthread_mutex_unlock(pthread_mutex_t *mutex);
    函数功能：对互斥量进行解锁
    函数参数
    - mutex：需要进行解锁的互斥量
    返回值
    - 成功：0
    - 失败
        - EINVAL：mutex 没有被初始化
        - EPERM：调用解锁的线程没有 mutex 互斥量

    TIPS：加锁和解锁，都是互斥量的一种状态转换
*/

/*
    - 在主线程中创建多个子线程
    - 初始化共享资源（这里也就是初始化票的总数）
    - 在子线程中，使用互斥量，访问共享资源（原子操作，这里也就是访问存储了票总数的变量）
*/
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>


// 共享资源（票的总数）
int tickets = 20;

// 定义互斥量
pthread_mutex_t mutex;

// 子线程售票逻辑
void* sell_tickets(void* arg) {
    while (1) {
        // 访问共享资源，锁定互斥量
        pthread_mutex_lock(&mutex);
        if (tickets > 0) {
            --tickets;
            printf("tid = %ld is selling %d number ticket.\n", pthread_self(), 20 - tickets);
        }
        else {
            // 访问共享资源结束，解锁互斥量
            pthread_mutex_unlock(&mutex);
            break;
        }

        // 访问共享资源结束，解锁互斥量
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }

    // 子线程退出
    return NULL;        // 等价于 pthread_exit(NULL)
}

int main(int argc, char* argv[]) {

    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    // 创建三个子线程
    for (int i = 0;i < 3;++i) {
        pthread_t tid;
        pthread_attr_t attr;

        // 初始化线程属性
        int ret1 = pthread_attr_init(&attr);
        if (ret1 != 0) {
            char* attr_error = strerror(ret1);
            printf("pthread_attr_init: %s\n", attr_error);
        }

        // 设置线程分离
        int ret2 = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        int ret3 = pthread_create(&tid, &attr, sell_tickets, NULL);

        // 释放线程属性资源
        int ret4 = pthread_attr_destroy(&attr);
    }

    // 主线程退出
    pthread_exit(NULL);

    // 释放互斥量资源
    //pthread_mutex_destroy(&mutex);

    return 0;
}