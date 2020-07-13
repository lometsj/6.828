#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



void prime(int left){
    // printf("what\n");
    int num;
    read(left, &num, sizeof(num));
    printf("prime %d\n", num);
    int buf;
    int right[2];
    int flag = 1;
    pipe(right);
    while(read(left, &buf, sizeof(buf)) > 0){
        if(buf % num != 0){
            write(right[1], &buf, sizeof(buf));
            if(flag){
                flag = 0;
                if(fork() == 0){
                    close(right[1]);
                    prime(right[0]);
                    exit();
                }
            }
        }
    }
    // printf("end %d\n",num);
    close(right[1]);
}


int main(){
    int feed_fd[2];
    pipe(feed_fd);
    //close(feed_fd[0]);

    if(fork() == 0){
        close(feed_fd[1]);
        prime(feed_fd[0]); 
    } else {
        close(feed_fd[0]);
        for(int i = 2;i <= 35 ;i++){
            write(feed_fd[1], &i, sizeof(i));
        }
        close(feed_fd[1]);
    }
    wait();
    exit();
}