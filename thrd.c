#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
int check_exit(int status)
{
    int flag = 1;
    if(WIFEXITED(status) == 0)
    {
        flag = 0;
    }
    if(WEXITSTATUS(status) == 1)
    {
        flag = 0;
    }
    return flag;
}
int main(int argc, char** argv)
{
    if(argc != 6)
    {
        exit(0);
    }
    char* cmd1 = argv[1];
    char* cmd2 = argv[2];
    char* cmd3 = argv[3];
    char* f1 = argv[4];
    char* f2 = argv[5];
    int status;
    int fd[2];
    int fd1[2];
    pipe(fd);
    pipe(fd1);
    if(fork() == 0)
    {
        int fd_inp = open(f1, O_RDONLY, 0666);
        if(fd_inp < 0)
        {
            exit(1);
        }
        int fd_out = open(f1, O_WRONLY|O_CREAT, 0666);
        if(fd_out < 0)
        {
            exit(1);
        }
        printf("started\n");
        if(fork == 0)
        {
            fd_inp = dup(0);
            fd[1] = dup(1);
            execlp(cmd1, cmd1, NULL);
            exit(1);
        }
        else
        {
            wait(&status);
        }
        printf("frst passed\n");
        if(fork() == 0)
        {
            fd1[0] = dup(0);
            fd[1] = dup(1);
            execlp(cmd2, cmd2, NULL);
            exit(1);
        }
        printf("scnd passed\n");
        if(fork() == 0);
        {
            fd_out = dup(1);
            fd[0] = dup(0);
            execlp(cmd3, cmd3, NULL);
            exit(1);
        }
        printf("thrd passed\n");
        close(fd_inp);
        close(fd_out);
    }
    wait(&status);
    wait(&status);
    exit(0);
}