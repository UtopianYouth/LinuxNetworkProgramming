/*
    #include <pthread.h>
    int pthread_detach(pthread_t thread);

    函数功能：分离一个线程
    - 被分离的线程运行结束后，资源会被 OS 自动释放，无需其他线程调用 pthread_join()
    - 不能通过 pthread_join() 函数去连接一个已经分离的线程
    函数参数
    - thread：需要分离的线程 ID
    返回值
    - 成功：0
    - 失败：非 0 值，错误号

*/

#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

void* callback(void* arg) {
    printf("child thread id = %ld\n", pthread_self());
    sleep(2);
    pthread_exit(NULL);     // 利用全局变量记录线程函数栈空间的内容

    //return NULL;    //等价于 pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

    // 创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if (ret != 0) {
        char* create_error = strerror(ret);
        printf("create error: %s\n", create_error);
        exit(0);
    }

    // 输出主线程和子线程的ID
    printf("child thread id = %ld, main thread id = %ld\n", tid, pthread_self());

    // 设置子线程分离，分离后，子线程对应的资源就不需要主线程释放
    ret = pthread_detach(tid);
    if (ret != 0) {
        char* detach_error = strerror(ret);
        printf("detach error: %s\n", detach_error);
    }

    // 对分离的子线程进行连接
    ret = pthread_join(tid, NULL);
    if (ret != 0) {
        char* join_error = strerror(ret);
        printf("join error: %s\n", join_error);
    }

    // 退出主线程
    pthread_exit(NULL);

    return 0;
}