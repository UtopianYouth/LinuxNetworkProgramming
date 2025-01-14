/*
    生产者消费者模型（粗略实现）

    操作对象
    - 生产者
    - 容器（限制容量）
    - 消费者

    通过条件变量和信号量解决关键问题
    - 容器满时，生产者进程阻塞，通知消费者进程消费
    - 容器空时，消费者进程阻塞，通知生产者进程生产

*/
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

// 链表作为容器
typedef struct Node {
    int data;
    struct Node* next;
}Node;

// 创建头节点
Node* create_head() {
    Node* head = (Node*)malloc(sizeof(Node));
    head->data = -1;
    head->next = NULL;
    return head;
}

// 链表容器，全局变量（共享资源）
Node* head = NULL;


// 创建互斥量
pthread_mutex_t mutex;

void* producer(void* arg) {
    // 生产者不断的创建新的节点，添加到链表中
    while (1) {
        sleep(1);

        pthread_mutex_lock(&mutex);
        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->data = random() % 1000;
        new_node->next = head->next;
        head->next = new_node;
        printf("producer p_tid = %ld, adding node num = %d\n", pthread_self(), new_node->data);
        pthread_mutex_unlock(&mutex);

    }
    return NULL;
}

void* consumer(void* arg) {
    // 消费者不断从链表头部取节点
    while (1) {
        sleep(1);

        pthread_mutex_lock(&mutex);
        if (head->next != NULL) {
            Node* tmp = head->next;
            head->next = head->next->next;
            printf("consumer c_tid = %ld, deleting node num = %d\n", pthread_self(), tmp->data);
            free(tmp);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    // 初始化链表头节点
    head = create_head();

    // 创建5个生产者线程，5个消费者线程
    pthread_t p_tids[5], c_tids[5];
    for (int i = 0;i < 5;++i) {
        pthread_create(p_tids + i, NULL, producer, NULL);
        pthread_create(c_tids + i, NULL, consumer, NULL);
    }

    // 线程分离
    for (int i = 0;i < 5;++i) {
        pthread_detach(p_tids[i]);
        pthread_detach(c_tids[i]);
    }

    // 保证主线程不退出
    while (1) {
        sleep(10);
    }

    // 释放互斥量
    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);
    return 0;
}