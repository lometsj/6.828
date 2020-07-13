#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    int num;
    if(argc <= 1){
        sleep(0);
        exit();
    } else {
        num = atoi(argv[1]);
        sleep(num);
        exit();
    }

}