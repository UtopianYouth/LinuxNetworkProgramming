/*
    #include <pthread.h>

    线程属性类型：pthread_attr_t

    int pthread_attr_init(pthread_attr_t *attr);
    函数功能：初始化线程属性变量
    函数参数
    - attr：传出参数，线程属性变量
    返回值
    - 成功：0
    - 失败：非 0 错误号

    int pthread_attr_destroy(pthread_attr_t *attr);
    函数功能：释放线程属性的资源
    函数参数
    - attr：传出参数，线程属性变量
    返回值
    - 成功：0
    - 失败：非 0 的错误号

    int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
    函数功能：获取线程分离的状态属性
    函数参数：
    - attr：线程属性对象
    - detachstate：存储获取到的线程状态，常见的两种：PTHREAD_CREATE_JOINABLE, PTHREAD_CREATE_DETACHED
    返回值
    - 成功：0
    - 失败：非 0 的错误号

    int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
    函数功能：设置线程分离的状态属性
    函数参数
    - attr：线程属性对象
    - detachstate：创建线程之前，需要设置的线程分离状态（属于线程的属性之一），常见的两种：PTHREAD_CREATE_JOINABLE, PTHREAD_CREATE_DETACHED
    返回值
    - 成功：0
    - 失败：非 0 的错误号
*/

#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

void* callback(void* arg) {
    printf("child thread id = %ld\n", pthread_self());
    return NULL;    //等价于 pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

    // 创建一个线程属性变量
    pthread_attr_t attr;

    // 初始化线程属性变量
    pthread_attr_init(&attr);

    // 设置线程属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // 创建一个子线程
    pthread_t tid;
    int ret1 = pthread_create(&tid, &attr, callback, NULL);
    if (ret1 != 0) {
        char* create_error = strerror(ret1);
        printf("create error: %s\n", create_error);
        exit(0);
    }

    // 获取子线程栈空间的大小
    size_t size;
    int ret2 = pthread_attr_getstacksize(&attr, &size);
    if (ret2 != 0) {
        char* get_stack_attr_error = strerror(ret2);
        printf("get_stack_attr_error: %s\n", get_stack_attr_error);
    }
    else {
        printf("child thread stack size = %ld\n", (long)size);
    }

    // 获取子线程的线程分离属性
    int val = 0;
    int ret3 = pthread_attr_getdetachstate(&attr, &val);
    if (ret3 != 0) {
        char* get_detach_attr_error = strerror(ret3);
        printf("get_detach_attr_error: %s\n", get_detach_attr_error);
    }
    else {
        printf("child thread detach_attr: %d\n", val);
    }

    // 释放线程属性资源
    pthread_attr_destroy(&attr);

    // 退出主线程
    pthread_exit(NULL);

    return 0;
}