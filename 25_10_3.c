#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
    pid_t p;
    int fd[2];
    pipe(fd);
    int fd_wr = open(argv[5], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
    int status;
    dup2(fd_wr, fd[1]);
    if((p = fork()) == 0)
    {
        close(fd[0]);
        dup2(fd[1], fd_wr);
        execlp(argv[1], argv[1], argv[2], NULL);
        close(fd[1]);
        close(fd_wr);
        exit(1);
    }
    else
    {
        exit(0);
    }
    wait(&status);
    if((p = fork()) == 0)
    {
        close(fd[0]);
        dup2(fd[1], fd_wr);
        execlp(argv[3], argv[3], argv[4], NULL);
        close(fd[1]);
        close(fd_wr);
        exit(1);
    }
    else
    {
        exit(0);
    }
    wait(&status);
    close(fd_wr);
    return 0;
}