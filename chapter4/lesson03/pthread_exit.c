/*
    #include <pthread.h>

    void pthread_exit(void *retval);

    函数功能：终止调用该函数的线程，终止的线程，在同一进程中的其他线程，可以通过 pthread_join(3) 加入
    函数参数
    - retval：传出参数，指针类型，作为一个返回值，可以在 pthread_join() 中获取到

    pthread_t pthread_self(void);
    函数功能：获取调用线程的线程 ID
    返回值：线程 ID

    int pthread_equal(pthread_t t1, pthread_t t2);
    函数功能：比较两个线程 ID 是否相等
    函数参数
    - t1：比较的第一个线程 ID
    - t2：比较的第二个线程 ID
    返回值
    - 相等：非零的值
    - 不相等：0
    TIPS: 不同的 OS，pthread_t 类型的实现不一样，有的是无符号长整数，有的是结构体，所以不能直接使用 == 比较

*/

#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>

void* callback(void* arg) {
    printf("child thread id = %ld\n", pthread_self());
    return NULL;
}

int main(int argc, char* argv[]) {

    // 创建一个子线程
    pthread_t tid;

    int ret = pthread_create(&tid, NULL, callback, NULL);

    if (ret != 0) {
        char* error = strerror(ret);
        exit(0);
    }

    for (int i = 0;i < 5;++i) {
        printf("%d\n", i);
    }
    printf("parent thread id = %ld, child thread id = %ld\n", pthread_self(), tid);

    // 让主线程退出，主线程退出不会影响其他线程的运行
    pthread_exit(NULL);

    printf("parent thread...\n");

    return 0;
}