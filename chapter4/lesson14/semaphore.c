/*
    信号量

    信号量的类型：sem_t
    #include <semaphore.h>
    int sem_init(sem_t *sem, int pshared, unsigned int value);
    函数功能：初始化信号量
    函数参数
    - sem：需要初始化的信号量对象
    - pshared：赋值为 0，表示线程之间的信号量，赋值为非 0，表示进程之间的信号量
    - value：信号量的值，可以理解为资源的个数
    返回值
    - 成功：0
    - 失败：-1，并且设置错误号

    int sem_wait(sem_t *sem);
    函数功能：申请一个信号量资源，对信号量加锁，并且进行减一操作，如果 sem->value <= 0，调用该函数的线程阻塞
    函数参数
    - sem：信号量资源对象
    返回值
    - 成功：0
    - 失败：-1，并且设置错误号，信号量对象 sem 的 value 值不改变

    int sem_trywait(sem_t *sem);
    函数功能：和 sem_wait() 一样，申请一个信号量资源，不同的是，如果 sem->value <= 0，不会阻塞线程，直接返回 EAGAIN 错误号
    函数参数
    - sem：信号量资源对象
    返回值
    - 成功：0
    - 失败：-1，并且设置相应的错误号，信号量对象 sem 的 value 值不改变

    int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
    函数功能：和 sem_wait() 一样，申请一个信号量资源，不同的是，如果 sem->value <= 0，阻塞线程，并且 abs_timeout 指定阻塞的时间
    - 如果在 abs_timeout 指定的时间内，sem->value 的值还是 <=0，结束线程阻塞，返回 ETIMEDOUT 错误号
    - 如果在 abs_timeout 指定的时间内，sem->value 的值 >0，进行信号量资源减一
    函数参数
    - sem：信号量资源对象
    - abs_timeout：指定线程能够接受最大的阻塞时间
    返回值
    - 成功：0
    - 失败：-1，并且设置相应的错误号，信号量对象 sem 的 value 值不改变

    int sem_post(sem_t *sem);
    函数功能：释放一个信号量资源，对信号量解锁，并且进行加一操作，对因为调用 sem_wait() 阻塞的线程进行唤醒
    函数参数
    - sem：信号量资源对象
    返回值
    - 成功：0
    - 失败：-1，并且设置错误号，信号量对象 sem 的 value 值不改变

    int sem_getvalue(sem_t *sem, int *sval);
    函数功能：获取信号量对象的 value 值，赋值给 sval
    - 如果存在因为 sem 对象阻塞的线程，POSIX.1 允许 sval 的值有两种情况，可以是 0，也可以是 value 的绝对值（value 为负数时，说明存在阻塞进程）
    函数参数
    - sem：信号量对象
    - sval：接收 sem->value 的值
    返回值
    - 成功：0
    - 失败：-1，并且设置错误号 EINVAL，表示无效的 sem

    int sem_destroy(sem_t *sem);
    函数功能：释放信号量资源
    函数参数
    - sem：需要释放的信号量对象
    返回值
    - 成功：0
    - 失败：-1，并且设置错误号

*/

/*
    使用信号量实现：生产者-消费者模型
*/
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<stdlib.h>

// 定义互斥量，保证线程数据安全（线程同步，保证同一时刻，只能有一个线程操作链表）
pthread_mutex_t mutex;

// 定义信号量
sem_t consumer_sem;
sem_t producer_sem;

// 定义共享资源
typedef struct LinkNode {
    int data;
    struct LinkNode* next;
}LinkNode, LinkList;

LinkList* head = NULL;

void createLinkList() {
    head = (LinkNode*)malloc(sizeof(LinkNode));
    head->data = -1;
    head->next = NULL;
}


// 消费者子线程
void* consumer(void* arg) {
    // 消费者不断从链表头部取节点
    while (1) {
        sleep(1);
        // 消费者信号量减一
        sem_wait(&consumer_sem);
        pthread_mutex_lock(&mutex);

        // 通过信号量机制，不需要判断链表中是否有节点
        LinkNode* tmp = head->next;
        head->next = tmp->next;
        printf("consumer thread, LinkNode value = %d\n", tmp->data);
        free(tmp);

        pthread_mutex_unlock(&mutex);
        // 生产者信号量加一
        sem_post(&producer_sem);
    }
    return NULL;
}

// 生产者子线程
void* producer(void* arg) {
    // 生产者不断从链表头部加节点
    while (1) {
        sleep(1);
        // 生产者信号量减一
        sem_wait(&producer_sem);
        pthread_mutex_lock(&mutex);

        LinkNode* tmp = (LinkNode*)malloc(sizeof(LinkNode));
        tmp->data = rand() % 1000;
        tmp->next = head->next;
        head->next = tmp;
        printf("producer thread, LinkNode value = %d\n", tmp->data);

        pthread_mutex_unlock(&mutex);
        // 消费者信号量加一
        sem_post(&consumer_sem);
    }
    return NULL;
}

int main() {

    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    // 初始化信号量
    sem_init(&consumer_sem, 0, 0);
    sem_init(&producer_sem, 0, 8);      // 生产者信号量的 value 值，决定了容器的上限

    // 创建共享资源
    createLinkList();

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

    // 释放信号量
    sem_destroy(&producer_sem);
    sem_destroy(&consumer_sem);

    pthread_exit(NULL);
    return 0;
}