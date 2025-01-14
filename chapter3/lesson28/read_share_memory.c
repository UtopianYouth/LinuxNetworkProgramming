#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char* argv[]) {
    // 创建共享内存
    int shm_id = shmget(100, 0, IPC_CREAT);
    printf("read shm_id = %d\n", shm_id);

    // 关联共享内存
    void* shm_ptr = shmat(shm_id, NULL, 0);

    // 读取共享内存中的信息
    char buf[4096] = { '\0' };
    strcpy(buf, (char*)shm_ptr);
    printf("read shm: %s", buf);

    printf("press any key to exit read process\n");
    getchar();

    // 解除关联
    shmdt(shm_ptr);

    // 删除共享内存
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}