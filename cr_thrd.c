#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

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
    if(argc != 4)
    {
        exit(1);
    }
    char* cmd1 = argv[1];
    char* cmd2 = argv[2];
    char* cmd3 = argv[3];
    int status;
    if(fork() == 0)
    {
        execlp(cmd1, cmd1, NULL);
    }
    wait(&status);
    int flag = check_exit(status);
    if(flag == 1)
    {
        if(fork() == 0)
        {
            execlp(cmd3, cmd3, NULL);
        }
        wait(&status);
        int flag = check_exit(status);
        if(flag == 0)
        {
            exit(1);
        }
        else
        {
            exit(0);
        }
    }
    else
    {
        if(fork() == 0)
        {
            execlp(cmd2, cmd2, NULL);
        }
        wait(&status);
        int flag = check_exit(status);
        if(flag == 1)
        {
            if(fork() == 0)
            {
                execlp(cmd3, cmd3, NULL);
            }
            wait(&status);
            int flag = check_exit(status);
            if(flag == 0)
            {
                exit(1);
            }
        }
        else
        {
            exit(1);
        }
    }
}
