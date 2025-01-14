/*
    条件变量（不是锁，主要用途是基于条件阻塞和唤醒线程，结合互斥量使用）

    条件变量的类型：pthread_cond_t

    int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr);
    函数功能：初始化条件变量
    函数参数
    - cond：需要进行初始化的条件变量
    - cond_attr：条件变量初始化的属性对象，默认NULL即可
    返回值
    - 成功：0
    - 失败：非 0 的错误号

    int pthread_cond_signal(pthread_cond_t *cond);
    函数功能：唤醒一个或多个等待的线程
    函数参数
    - cond：条件变量对象
    返回值
    - 成功：0
    - 失败：非 0 的错误号

    int pthread_cond_broadcast(pthread_cond_t *cond);
    函数功能：唤醒所有等待的线程
    函数参数
    - cond：条件变量对象
    返回值
    - 成功：0
    - 失败：非 0 的错误号

    int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
    函数功能：阻塞，调用该函数的线程会阻塞，但是会将互斥量 mutex 的状态设置为 unlock，其他线程可以获得互斥量
    函数参数
    - cond：条件变量对象
    - mutex：互斥量对象
    返回值
    - 成功：0
    - 失败：非 0 的错误号

    int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
    函数功能：线程阻塞指定的时间，调用该函数的线程，会阻塞，直到指定的时间结束
    函数参数
    - cond：条件变量对象
    - mutex：互斥量对象
    - abstime：设置线程阻塞的指定时间
    返回值
    - 成功：0
    - 失败：非 0 的错误号

    int pthread_cond_destroy(pthread_cond_t *cond);
    函数功能：释放条件变量资源
    函数参数
    - cond：需要释放的条件变量对象
    返回值
    - 成功：0
    - 失败：非 0 的错误号

*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

// 定义条件变量
pthread_cond_t condition;

// 定义互斥量
pthread_mutex_t mutex;

// 定义共享资源，链表头部添加删除节点
typedef struct LinkNode {
    int data;
    struct LinkNode* next;
}LinkNode, LinkList;

LinkList* head = NULL;

// 创建链表，初始化头节点
void createLinkList() {
    head = (LinkNode*)malloc(sizeof(LinkNode));
    head->data = -1;
    head->next = NULL;
}


// 消费者子线程
void* consumer(void* arg) {

    // 消费者线程不断从链表头部取节点
    while (1) {
        sleep(1);
        pthread_mutex_lock(&mutex);
        if (head->next != NULL) {
            LinkNode* tmp = head->next;
            head->next = tmp->next;
            printf("consumer thread, LinkNode data = %d\n", tmp->data);
            free(tmp);
        }
        else {
            // 链表中没有节点，使用条件变量阻塞消费者进程，并且释放互斥量的锁，避免 CPU 资源的浪费
            // 执行该函数的线程，会使互斥量解锁，相当于执行 pthread_mutex_unlock() 操作，被唤醒后，又会对互斥量上锁
            pthread_cond_wait(&condition, &mutex);
        }
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

// 生产者子线程
void* producer(void* arg) {

    // 生产者不断从链表头部插入节点
    while (1) {
        sleep(1);
        pthread_mutex_lock(&mutex);
        LinkNode* tmp = (LinkNode*)malloc(sizeof(LinkNode));
        tmp->data = rand() % 1000;
        tmp->next = head->next;
        head->next = tmp;
        printf("producer thread, LinkNode data = %d\n", tmp->data);

        // 生产了链表节点，可以唤醒消费者进程
        pthread_cond_signal(&condition);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}


int main(int argc, char* argv[]) {

    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    // 初始化条件变量
    pthread_cond_init(&condition, NULL);

    // 初始化共享资源
    createLinkList();

    // 创建5个消费者线程和5个生产者线程
    pthread_t c_tid[5], p_tid[5];
    for (int i = 0;i < 5;++i) {
        pthread_create(p_tid + i, NULL, producer, NULL);
        pthread_create(c_tid + i, NULL, consumer, NULL);
    }

    // 设置线程分离
    for (int i = 0;i < 5;++i) {
        pthread_detach(p_tid[i]);
        pthread_detach(c_tid[i]);
    }

    // 主线程不退出
    while (1) {
        sleep(1);
    }

    // 释放互斥量
    pthread_mutex_destroy(&mutex);

    // 释放条件变量
    pthread_cond_destroy(&condition);

    return 0;
}