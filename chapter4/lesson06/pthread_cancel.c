/*
    #include <pthread.h>
    int pthread_cancel(pthread_t thread);

    函数功能：取消线程（让线程终止运行）
    - 被指定，线程取消的线程，并不会立马终止运行，而是执行到一个 pthread 指定的 cancellation point，线程才会终止
    - cancellation point：可以理解为系调用（线程进入内核态）
    函数参数
    - thread：需要进行线程取消的线程 ID
    返回值：
    - 成功：0
    - 失败：非 0 值，错误号


*/

#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

// 子线程代码逻辑
void* callback(void* arg) {
    for (int i = 0; i < 5; ++i) {
        printf("child thread: %d\n", i);
    }
    return NULL;    //等价于 pthread_exit(NULL);
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

    // 取消线程
    pthread_cancel(tid);

    // 输出主线程的 ID
    for (int i = 0;i < 5;++i) {
        printf("main thread: %d\n", i);
    }

    // 退出主线程
    pthread_exit(NULL);

    return 0;
}