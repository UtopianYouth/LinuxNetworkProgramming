#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

/*
    聊天a进程
    - 以只读的方式打开b_to_a有名管道
    - 以只写的方式打开a_to_b有名管道
    - 循环写入数据
    while(1){
        获取键盘事件，fgets
        写管道
        读管道
    }
*/

int main(int argc, char* argv[]) {

    /*
        有名管道注意事项，任意一个进程以只读或只写的方式打开有名管道，
        如果没有其他对应进程以只写或只读打开有名管道，都会被阻塞
    */

    // 1. 只写方式打开 a_to_b
    int a_to_b_fd = open("./a_to_b", O_WRONLY);

    // 2. 只读方式打开 b_to_a
    int b_to_a_fd = open("./b_to_a", O_RDONLY);

    // 管道文件打开失败
    if (a_to_b_fd == -1 || b_to_a_fd == -1) {
        printf("end chat_a process.\n");
        exit(0);
    }

    printf("open a_to_b and b_to_a successful...\n");

    while (1) {
        char buf_read[1024] = { '\0' };
        char buf_write[1024] = { '\0' };

        // 3. 获取键盘写入的数据
        fgets(buf_write, 1024, stdin);

        // 4. 向 a_to_b 管道写数据
        int ret1 = write(a_to_b_fd, buf_write, sizeof(buf_write));

        if (ret1 == -1) {
            // 管道读端进程关闭，写入数据失败
            perror("write");
            break;
        }

        // 5. 向 b_to_a 管道读数据
        int ret2 = read(b_to_a_fd, buf_read, sizeof(buf_read));
        if (ret2 == -1) {
            // 管道写端进程关闭，读取数据失败
            perror("read");
            break;
        }
        printf("b_to_a info: %s", buf_read);
    }
    // 6. 关闭文件描述符对应的管道
    close(a_to_b_fd);
    close(b_to_a_fd);

    printf("end chat_a process.\n");

    return 0;
}