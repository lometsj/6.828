#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    int parent_fd[2], child_fd[2];
    if(pipe(parent_fd) < 0){
        goto error;
    }
    if(pipe(child_fd) < 0){
        goto error;
    }

    char data = 'a';

    int fpid = fork();
    if(fpid == 0){
        read(parent_fd[0], &data, 1);
        printf("%d: received ping\n", getpid());

        write(child_fd[1], &data, 1);
    } else {
        write(parent_fd[1], &data, 1);

        read(child_fd[0], &data, 1);
        printf("%d: received pong\n", getpid());
    }
    wait();
    exit();
    error:
        printf("error");
        exit();
}