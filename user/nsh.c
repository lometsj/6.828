#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"


//echo hello goodbye\n  hello goodbye
// grep constitute README\n
// The code in the files that constitute xv6 is

// echo x\necho goodbye\n
// goodbye

// redirect > <  both

// pipe

// re + pipe


// token: cmd, >, <, |, 
//         1   2  3  4  

char buf[512];
char *p;

// run cmd by exec syscall
void execCmd(char *cmd){
    // fprintf(2,"execCmd:%s",cmd);
    // char binary[128];
    char arg[MAXARG][MAXARG];
    char *p = cmd;
    char *argp[MAXARG];
    // int flag = 1;
    int idx = 0;
    while(1){
        if(*p == '\x00'){
            break;
        }
        if(*p == ' '){
            p++;
            continue;
        }
        // printf("?:%s",p);
        char *i; 
        for(i = p; *i!=' ' && *i!='\x00' ;i++);
        *i = '\x00';
        strcpy(arg[idx++], p);
        p = i+1;
    }
    for(int i = 0;i<idx;i++){
        argp[i] = arg[i];
        // printf("%d : %s\n",i,arg[i]);
    }
    exec(argp[0], argp);
    return;
}


void getFilename(char *text, char *retbuf){
    char *t = text;
    for(;*t == ' ';t++);
    int i;
    for(i = 0;i< strlen(t);i++){
        if(t[i]!=' ' && t[i]!='>' && t[i]!= '<' && t[i]!='\x00')continue;
        else break;
    }
    memmove(retbuf, t, i);
    retbuf[i] = '\x00';
    // printf("filename: %s\n",retbuf);
    return;
}

void runcmd(char *cmd){
    // fprintf(2,"runcmd:%s\n",cmd);
    char *p = strchr(cmd, '|');
    if(p != 0){
        *p = '\x00';
        char lcmd[MAXARG] = {0};
        char rcmd[MAXARG] = {0};
        strcpy(lcmd, cmd);
        strcpy(rcmd, p+1);
        int fd[2];
        pipe(fd);
        
        if(fork()){
            close(fd[0]);
            close(1);
            dup(fd[1]);
            runcmd(lcmd);
        }else{
            close(fd[1]);
            close(0);
            dup(fd[0]);
            runcmd(rcmd);
        }
        wait(0);
        return;
    }
    // fprintf(2,"enter pipe over:%s\n",cmd);
    char *p1 = strchr(cmd, '>');
    char *p2 = strchr(cmd, '<');

    char filename[32] = {0};
    int file_fd;
    if(p1){
        // fprintf(2,"!1\n");
        *p1 = '\x00';
        getFilename(p1+1, filename);
        file_fd = open(filename, O_CREATE | O_RDWR);
        close(1);
        dup(file_fd);
        memset(filename, 0, 32);
    }
    if(p2){
        // fprintf(2,"!2\n");

        *p2 = '\x00';
        getFilename(p2+1, filename);
        // fprintf(2,"out: %s",filename);
        file_fd = open(filename, O_RDONLY);
        close(0);
        dup(file_fd);
    }
    if(p1 || p2){
        // fprintf(2,"!3\n");

        // printf("????");
        char leftCmd[512] = {0};
        strcpy(leftCmd, cmd);
        execCmd(leftCmd);
        return;
    }
    // fprintf(2,"!4\n");
    // printf("!!!!!!!!!!!!!!!!!!!!!");
    execCmd(cmd);
    return;
}

void getline(){
    printf("@ ");  //print prompt
    gets(buf, 512);
    for(int i=0;i<512;i++){
        if(buf[i] == '\n') buf[i] = '\x00';
    }
    return;
}



int main(){

    if(open("console",O_RDONLY) != 3){
        fprintf(2, "stdin/stdout/stderr open error");
        exit(0);
    }
    close(3);

    while(1){
        getline();// input cmd by line
        if(buf[0] == '\x00')exit(0);
        if(fork()){
            wait(0);
            continue;
        }else{
            char cmd[512];
            strcpy(cmd, buf);
            runcmd(cmd);
        }

        
    }
    exit(0);
}