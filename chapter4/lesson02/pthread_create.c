/*
    一般情况下，main函数所在的线程，我们称之为主线程，其余创建的线程称之为子线程

    #include <pthread.h>

    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                        void *(*start_routine) (void *), void *arg);

    函数功能：创建一个子线程（Linux中，进程默认对应一个线程，创建子线程的叫做主线程）
    函数参数
    - thread：传出参数，线程创建成功后，子线程的线程 ID 被写到该变量
    - attr：设置线程属性，一般使用默认值，NULL
    - start_routine：函数指针，这个函数是子线程需要处理的逻辑代码
    - arg：给第三个参数使用，传参
    返回值
    - 成功：返回0
    - 失败：返回错误号，这个错误号和之前的 errno 有区别
    获取错误号的信息：char* strerror(int errnum)

*/
#include<stdio.h>
#include<pthread.h>
#include<string.h>  
#include<stdlib.h>
#include<unistd.h>


void* my_callback(void* arg);
int main() {

    pthread_t tid;
    int num = 10;

    // 创建一个子线程
    int ret = pthread_create(&tid, NULL, my_callback, (void*)&num);

    // 创建子线程失败，返回一个错误号
    if (ret != 0) {
        char* str = strerror(ret);
        exit(0);
    }

    // 主线程执行代码
    for (int i = 0;i < 5;++i) {
        printf("%d\n", i);
    }
    sleep(1);

    return 0;
}

void* my_callback(void* arg) {
    printf("child pthread...\n");
    printf("child pthread arg: %d\n", *(int*)arg);

    // 返回值是void* 类型
    return NULL;
}