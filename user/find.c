#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"



void find(char *path, char *target){
    char buf[512];
    int fd;
    char *p;
    struct stat st;
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
  }

  if(st.type == T_FILE){
      fprintf(2, "find: arg 1 not a path");
      close(fd);
      return;
  }else if(st.type == T_DIR){
      struct dirent de;
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
          fprintf(2, "find: path too long");
      }
      strcpy(buf, path);
      p = buf+strlen(buf);
      *p++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
          if(de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }
        if(st.type == T_FILE){
            if(strcmp(p, target) == 0)
                printf("%s\n",buf);
        }else if(st.type == T_DIR){
            if(strcmp(p, ".")==0 || strcmp(p, "..")==0){
                continue;
            }
            find(buf, target);
        }


      }
  }
}


int main(int argc, char *argv[]){
    if(argc == 1){
        find(".", "");
        exit();
    }
    if(argc == 2){
        find(argv[1], "");
        exit();
    }
    if(argc == 3){
        find(argv[1], argv[2]);
        exit();
    }
    exit();
}