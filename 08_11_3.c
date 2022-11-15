#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("arg err");
        exit(1);
    }
    pid_t p;
    int fd[2];
    int status;
    int fd_out = open(argv[2], O_RDWR, 0777);
    if(fd_out < 0)
    {
        exit(2);
    }
    pipe(fd);
    p = fork();
    if(p < 0)
    {
        printf("error fork");
        exit(2);
    }
    if(p == 0)
    {
        close(fd[0]);
        dup2(fd[1], 1);
        execlp("./08_11_1", "./08_11_1", argv[1], NULL);
        close(fd[1]);
        exit(1);
    }
    else
    {
        wait(&status);
        close(fd[1]);
        dup2(fd_out, 1);
        dup2(fd[0], 0);
        execlp("sort", "sort" ,NULL);
        close(fd[0]);
    }
    close(fd_out);
    wait(&status);
    return 0;
}