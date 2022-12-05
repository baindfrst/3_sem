#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

int now;
int take;
int fd[2];
pid_t son;
pid_t sson;
pid_t father;

void father_func(int sig)
{
    read(fd[0], &now, sizeof(int));
    read(fd[0], &take, sizeof(int));
    if(now == take)
    {
        printf("%d f: %d\n", getpid(), now);
        now++;
        write(fd[1], &now, sizeof(int));
        write(fd[1], &take, sizeof(int));
        kill(son, SIGUSR1);
        exit(1);
    }
    else
    {
        if(now == take + 1)
        {
            write(fd[1], &now, sizeof(int));
            write(fd[1], &take, sizeof(int));
            kill(son, SIGUSR1);
            exit(1);
        }
    }
    printf("%d f: %d\n", getpid(), now);
    now++;
    write(fd[1], &now, sizeof(int));
    write(fd[1], &take, sizeof(int));
    kill(son, SIGUSR1);
}

void son_func(int sig)
{
    read(fd[0], &now, sizeof(int));
    read(fd[0], &take, sizeof(int));
    if(now == take)
    {
        printf("%d s: %d\n", getpid(), now);
        now++;
        write(fd[1], &now, sizeof(int));
        write(fd[1], &take, sizeof(int));
        kill(sson, SIGUSR1);
        exit(1);
    }
    else
    {
        if(now == take + 1)
        {
            write(fd[1], &now, sizeof(int));
            write(fd[1], &take, sizeof(int));
            kill(sson, SIGUSR1);
            exit(1);
        }
    }
    printf("%d s: %d\n", getpid(), now);
    now++;
    write(fd[1], &now, sizeof(int));
    write(fd[1], &take, sizeof(int));
    kill(sson, SIGUSR1);
}

void son_of_son_func(int sig)
{
    read(fd[0], &now, sizeof(int));
    read(fd[0], &take, sizeof(int));
    if(now == take)
    {
        printf("%d ss: %d\n", getpid(), now);
        now++;
        write(fd[1], &now, sizeof(int));
        write(fd[1], &take, sizeof(int));
        kill(father, SIGUSR1);
        exit(1);
    }
    else
    {
        if(now == take + 1)
        {
            write(fd[1], &now, sizeof(int));
            write(fd[1], &take, sizeof(int));
            kill(father, SIGUSR1);
            exit(1);
        }
    }
    printf("%d ss: %d\n", getpid(), now);
    now++;
    write(fd[1], &now, sizeof(int));
    write(fd[1], &take, sizeof(int));
    kill(father, SIGUSR1);
}

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        perror("input data error\n");
        exit(0);
    }
    int to = atoi(argv[2]);
    int from = atoi(argv[1]);
    if(from > to)
    {
        return 0;
    }
    pipe(fd);
    write(fd[1], &from, sizeof(int));
    write(fd[1], &to, sizeof(int));
    son = fork();
    if(son == 0)
    {
        father = getppid();
        sson = fork();
        if(sson != 0)
        {
            signal(SIGUSR1, son_func);
        }
        else
        {
            signal(SIGUSR1, son_of_son_func);
            kill(father, SIGUSR1);
        }
    }
    else
    {
        signal(SIGUSR1, father_func);
    }
    while(1);
    return 0;
}