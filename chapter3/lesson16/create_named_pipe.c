#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>


int main(int argc, char* argv[]) {
    // 判断有名管道是否存在
    int is_exist = access("./a_to_b", F_OK);
    if (is_exist == -1) {
        // 不存在
        printf("a_to_b named pipe doesn't exist.\n");
        int ret = mkfifo("./a_to_b", 0664);

        if (ret == -1) {
            // 管道创建失败
            perror("mkfifo");
        }
    }

    // 判断有名管道是否存在
    is_exist = access("./b_to_a", F_OK);
    if (is_exist == -1) {
        // 不存在
        printf("b_to_a named pipe doesn't exist.\n");
        int ret = mkfifo("./b_to_a", 0664);

        if (ret == -1) {
            // 管道创建失败
            perror("mkfifo");
        }
    }
    return 0;
}