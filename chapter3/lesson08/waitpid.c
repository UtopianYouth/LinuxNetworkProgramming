/*
    #include <sys/types.h>
    #include <sys/wait.h>

    pid_t waitpid(pid_t pid, int *wstatus, int options);

    函数功能：父进程通过参数 pid 和 options，选择特定的方式回收子进程资源，可以设置父进程是否阻塞

    参数：pid_t pid
    - pid > 0 : 回收指定pid的子进程
    - pid = 0 : 回收当前进程组的所有子进程
    - pid = -1: 回收所有子进程资源，相当于wait()函数，最常用
    - pid < -1: 回收pgid = |pid|的所有子进程资源，限制了进程组范围

    参数：int* wstatus，进程退出时的状态信息，传入的是一个int类型的地址，传出参数

    参数：int options，设置父进程阻塞和非阻塞
    - 0 : 阻塞
    - WNOHANG : 非阻塞（如果不存在结束的子进程，waitpid()函数立即返回）

    返回值
    - > 0：返回回收资源的子进程id
    - = 0 : 如果 options = WNOHANG, 表示还有子进程没有运行结束
    - = -1 ：所有子进程都结束了且没有可以回收的子进程资源，或者是调用函数失败

*/
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[]) {

    pid_t pid;
    //创建5个子进程
    for (int i = 0;i < 5;++i) {
        pid = fork();
        if (pid == 0) {     //防止子进程再次创建子进程，孙子进程再次创建子进程（会一直嵌套执行下去）......
            break;
        }
        else if (pid < 0) {
            printf("create child process failure...\n");
        }
    }
    if (pid > 0) {
        //父进程
        while (1) {
            printf("parent process, pid = %d\n", getpid());
            int status;
            //int ret = waitpid(-1, &status, 0);    //options = 0，父进程阻塞
            int ret = waitpid(-1, &status, WNOHANG);      //options = WNOHANG，父进程不阻塞
            if (ret == -1) {
                printf("no child process need recycle\n");
                break;
            }
            else if (ret == 0) {
                printf("having child process\n");
            }
            else {
                printf("child process die, child pid = %d\n", ret);
                if (WIFEXITED(status)) {
                    //是不是正常退出
                    printf("退出的状态码：%d\n", WEXITSTATUS(status));
                }
                if (WIFSIGNALED(status)) {
                    //是不是异常终止
                    printf("异常终止状态码：%d\n", WTERMSIG(status));
                }
            }
            sleep(1);
        }
    }
    else if (pid == 0) {
        //子进程
        while (1) {
            printf("child process, pid = %d\n", getpid());
            sleep(1);
        }
    }
    return 0;
}