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
    pid_t p;
    int fd[2];
    int fd_wr = open(argv[5], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
    int status;
    if((p = fork()) == 0)
    {
        dup2(fd_wr, 1);
        close(fd_wr);
        execlp(argv[1], argv[1], argv[2], NULL);
        exit(0);
    }
    else
    {
        perror("fork");
    }
    wait(&status);
    if((p = fork()) == 0)
    {
        dup2(fd_wr, 1);
        close(fd_wr);
        execlp(argv[3], argv[3], argv[4], NULL);
        exit(0);
    }
    else
    {
        perror("fork");
    }
    wait(&status);
    return 0;
}