/*
    #include <pthread.h>
    int pthread_join(pthread_t thread, void **retval);

    函数功能：和一个已经终止的线程进行连接，回收子线程资源
    - 该函数是阻塞函数，调用一次只能回收一个子线程
    - 一般在主线程中使用
    函数参数
    - thread：需要回收子线程的 ID
    - retval：接收子线程退出时的返回值，使用二级指针的原因是，pthread_exit()的返回值是一级指针，作为传出参数
    需要得到一级指针的地址，才能对栈空间中的一级指针变量进行赋值修改（和使用自定义函数改变main函数中的局部变量原理一致）
    返回值
    - 成功：0
    - 失败：非 0 值，即错误号
*/

#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int child_thread;
void* callback(void* arg) {
    printf("child thread id = %ld\n", pthread_self());
    sleep(2);
    child_thread = 8;
    pthread_exit((void*)&child_thread);     // 利用全局变量记录线程函数栈空间的内容

    //return NULL;    //等价于 pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    // 创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);

    if (ret != 0) {
        char* error = strerror(ret);
        printf("%s\n", error);
        exit(0);
    }

    for (int i = 0;i < 5;++i) {
        printf("%d\n", i);
    }
    printf("parent thread id = %ld, child thread id = %ld\n", pthread_self(), tid);

    // 回收子线程资源
    int* ret_value;

    // 地址传递，才能改变 ret_value 的值
    int is_join = pthread_join(tid, (void**)&ret_value);
    if (is_join != 0) {
        char* error = strerror(is_join);
        printf("%s\n", error);
    }

    printf("recycle child thread success, recycle value = %d\n", *ret_value);

    // 让主线程退出
    pthread_exit(NULL);

    return 0;
}