#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{

 int fd[2];
 pipe(fd);
 if(fork()== 0) 
    {
        dup2(fd[1],1);
        close(fd[1]);
        close(fd[0]);
        execlp(�print�,�print�,0);
    }
    dup2(fd[0],0);
    close(fd[0]);
    close(fd[1]);
    execl(�/usr/bin/wc�,�wc�,0);
}
