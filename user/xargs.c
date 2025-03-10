#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include <stddef.h>
int main(int argc, char* argv[]){
    char buf[100]; 
    int byteread = read(0, buf, sizeof(buf)) - 1;
    buf[byteread] = '\0';
    //printf("buf:%s %d\n", buf, byteread);
    for(int l = 0, r = 0; r <= byteread; r++){
        if(buf[r]=='\n' || buf[r]=='\0'){  
            //printf("r:%d\n", r);  
            char* arg[4];
            arg[0] = argv[1];
            arg[1] = argv[2];
            arg[2] = "";
            arg[3] = NULL;
            memmove(arg[2], &buf[l], r-l);
            arg[2][r - l] = '\0';
            if(fork() > 0){//父进程
                wait(0);
            }else{
                //printf("argv1:%s, arg1%s , arg2%s\n", argv[1], arg[0], arg[1]);  
                //printf("buf:%s read:%d  arg[2]%s\n", buf, byteread, arg[2]);
                exec(argv[1], arg);
                //printf("\n");
                exit(0);
            }
            l=r+1;
        }
    }
    exit(0);
}
