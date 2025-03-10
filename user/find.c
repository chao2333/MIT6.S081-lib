#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}
void find(int fd, char* name, char* path2){ //name 为path path2为name
    struct stat st;
    struct dirent de;
    char buf[512], *p;
    strcpy(buf, name);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue; // 跳过 . 和 ..

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }

        if(st.type == T_FILE){
            if(strcmp(path2, p) == 0){
                printf("%s/%s\n", name, path2);
            }
        }else if(st.type == T_DIR){
            int new_fd = open(buf, 0);
            if (new_fd >= 0) {
                find(new_fd, buf, path2);
                close(new_fd);
            }

        }
    }
}
int main(int argc, char* argv[]){
    struct stat st;
    int fd = open(argv[1], 0);
    if(fd < 0){
        fprintf(2, "cannot open %s\n", argv[1]);
        exit(0);
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "cannot stat %s", argv[1]);
        exit(0);
    }

    switch(st.type){
        case T_DIR:
            find(fd, argv[1], argv[2]);
    }
    exit(0);
}