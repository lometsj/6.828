#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"


int main(int argc, char *argv[]){
    char buf[512], tmp;
    int flag = 0;
    char *p;
    char args[32][MAXARG] = {0};

    for(int i = 1;i < argc;i++){
        strcpy(args[i-1], argv[i]);
    }
readline:   
    p = buf;
    memset(buf, 0, 512);
    while(1){
        int ret = read(0, &tmp, 1);
        if(ret == 0){
            wait();
            exit();
        }
        if(tmp == '\n'){
            *p = 0;
            break;
        }
        *p++ = tmp;
        
    }

    int pid = fork();
    if(pid == 0){
        strcpy(args[argc-1], buf);
        args[argc][0] = '\x00';
        char *arg[MAXARG] = {0};
        for(int i = 0;i <= argc;i++)
            arg[i] = args[i];
        // printf("? %s",arg[argc-1]);
        exec(argv[1], arg);//to do
    }else{
        if(flag == 0){
            goto readline;
        }
    }
    exit();
}