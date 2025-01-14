// 共享内存相关函数
/*
    #include <sys/ipc.h>
    #include <sys/shm.h>
    int shmget(key_t key, size_t size, int shmflg);

    函数功能：创建一个新的共享内存段，或者获取一个既有的共享内存段的标识，
    新创建内存段中的数据都会被初始化为0
    函数参数
    - key：整型数据，通过 key 找到或者创建一个共享内存，一般使用16进制
    - size：共享内存的大小
    - shmflg：属性
        - 访问权限
        - 附加属性：创建/判断共享内存是不是存在
            - 创建：IPC_CREATE
            - 判断共享内存是否存在：IPC_EXCL，需要和IPC_CREATE一起使用
            - IPC_EXCL | IPC_CREATE
    返回值
    - 成功：>0，返回共享内存的引用ID，操作共享内存通过这个值
    - 失败：-1，设置错误号
*/

/*
    #include <sys/types.h>
    #include <sys/shm.h>
    void *shmat(int shmid, const void *shmaddr, int shmflg);

    函数功能：将共享内存和当前的进程进行关联
    函数参数
    - shmid：共享内存的标识（ID），由 shmget 返回值获取
    - shmaddr：申请的共享内存起始地址，一般指定NULL，由内核决定
    - shmflg：对共享内存的操作
        - 读权限：SHM_RDONLY，必须要有读权限
        - 读写权限：0
    返回值
    - 成功：返回共享内存的起始地址
    - 失败：返回(void*)-1，并且设置错误号
*/

/*
    #include <sys/types.h>
    #include <sys/shm.h>
    int shmdt(const void *shmaddr);

        函数功能：解除当前进程和共享内存的关联
        函数参数
        - shmaddr：共享内存的首地址
        返回值
        - 成功：返回0
        - 失败：返回-1，并且设置错误号
*/

/*
    #include <sys/ipc.h>
    #include <sys/shm.h>
    int shmctl(int shmid, int cmd, struct shmid_ds *buf);

    函数功能：删除共享内存，共享内存要删除才会消失，创建共享内存的进程被销毁，
    对共享内存没有任何影响
        函数参数
        - shmid：共享内存的ID
        - cmd：要做的操作
            - IPC_STAT：获取共享内存的当前状态
            - IPC_SET：设置共享内存的状态
            - IPC_RMID：标记当前进程，关联共享内存被解除（只有等到所有的进程都与共享内存解除关联了，共享内存才能被销毁）
            - IPC_INFO：
        - buf：需要设置或者获取共享内存中的属性信息
            - cmd = IPC_STAT：传出参数，获取共享内存的属性信息
            - cmd = IPC_SET：设置共享内存中的属性信息到内核中
            - cmd = IPC_RMID：设置NULL

*/
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char* argv[]) {
    // 创建共享内存
    int shm_id = shmget(100, 4096, IPC_CREAT | IPC_EXCL);
    printf("write shm_id = %d\n", shm_id);

    // 关联共享内存
    void* shm_ptr = shmat(shm_id, NULL, 0);

    // 向共享内存中写数据
    char* buf = "hello world\n";
    strcpy((char*)shm_ptr, buf);

    printf("press any key to exit write process\n");
    getchar();

    // 解除关联
    shmdt(shm_ptr);

    // 删除共享内存
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}