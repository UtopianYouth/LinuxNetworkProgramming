#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc, char* argv[]) {
    while (1) {
        printf("process id = %d\n", getpid());
    }
    return 0;
}