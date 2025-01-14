/*
    #include <signal.h>

    int sigemptyset(sigset_t *set);
    - 函数功能：清空阻塞信号集中的数据，将信号集中所有的标志位置为 0
    - 函数参数
        - set：传出参数，需要清空的信号集地址
    - 返回值：成功返回0，失败返回-1

    int sigfillset(sigset_t *set);
    - 函数功能：将阻塞信号集中所有的标志位置1
    - 函数参数
        - set：传出参数，需要填充的信号集地址
    - 返回值：成功返回0，失败返回-1

    int sigaddset(sigset_t *set, int signum);
    - 函数功能：设置阻塞信号集中的某一位对应的标志位为 1，表示阻塞这个信号
    - 函数参数
        - set：传出参数，需要操作的信号集
        - signum：需要设置阻塞的信号编号
    - 返回值：成功返回0，失败返回-1

    int sigdelset(sigset_t *set, int signum);
    - 函数功能：设置阻塞信号集中的某一位对应的标志位为 0，表示不阻塞这个信号
    - 函数参数
        - set：传出参数，需要操作的信号集
        - signum：需要设置不阻塞的信号编号
    - 返回值：成功返回0，失败返回-1

    int sigismember(const sigset_t *set, int signum);
    - 函数功能：判断某个信号是否阻塞
    - 函数参数
        - set：需要操作的信号集
        - signum：需要判断的那个信号编号
    - 返回值
        1：signum被阻塞
        0：signum不阻塞
        -1：函数调用失败

*/

#include<stdio.h>
#include<signal.h>

int main(int argc, char* argv[]) {
    sigset_t set;

    // 清空信号集中的内容
    sigemptyset(&set);

    // 判断 SIGINT 是否在信号集 set 里
    int ret1 = sigismember(&set, SIGINT);
    if (ret1 == 0) {
        printf("SIGINT 不阻塞\n");
    }
    else {
        printf("SIGINT 阻塞\n");
    }

    // 添加几个信号到信号集中
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);

    // 判断 SIGINT 是否在信号集 set 里
    int ret2 = sigismember(&set, SIGINT);
    if (ret2 == 0) {
        printf("SIGINT 不阻塞\n");
    }
    else {
        printf("SIGINT 阻塞\n");
    }

    // 判断 SIGQUIT 是否在信号集 set 里
    int ret3 = sigismember(&set, SIGQUIT);
    if (ret3 == 0) {
        printf("SIGQUIT 不阻塞\n");
    }
    else {
        printf("SIGQUIT 阻塞\n");
    }

    // 从信号集中删除一个信号
    sigdelset(&set, SIGQUIT);

    // 判断 SIGQUIT 是否在信号集 set 里
    int ret4 = sigismember(&set, SIGQUIT);
    if (ret4 == 0) {
        printf("SIGQUIT 不阻塞\n");
    }
    else {
        printf("SIGQUIT 阻塞\n");
    }

    return 0;
}