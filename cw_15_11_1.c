#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>
#define MAXLEN 40 // не совсем понял, что за MAXLEN
pid_t pid;
int file;
void printfather(int sig)
{
    puts("Father");
    raise(SIGUSR2);
}
void filefather(int sig)
{
    char readed;
    int ch;
    int len = 0;
    while((ch = read(file, &readed, 1)) > 0 && readed != '\n' && len != MAXLEN)
    {
        putchar(readed);
        len++;
    }
    if(ch == 0)
    {
        raise(SIGINT);
    }
    putchar('\n');
    kill(pid, SIGUSR1);
}
void fatheralarm(int sig)
{
    exit(1);
}
void fileson(int sig)
{
    char readed;
    int ch;
    int len = 0;
    while((ch = read(file, &readed, 1)) > 0 && readed != '\n' && len != MAXLEN)
    {
        putchar(readed);
        len++;
    }
    if(ch == 0)
    {
        raise(SIGINT);
    }
    putchar('\n');
    kill(getppid(), SIGUSR1);
}

void printson(int sig)
{
    puts("Son");
    raise(SIGUSR2);
}

void sonalarm(int sig)
{
    exit(1);
}

int main(int argc, char** argv)
{
    char c = 'p';
    if (argc != 2)
    {
        printf("error arg\n");
        exit(1);
    }
    time_t start_time;
    file = open(argv[1], O_RDONLY|O_CREAT);
    pid = fork();
    if(pid > 0)
    {
        signal(SIGUSR1, printfather);
        signal(SIGUSR2, filefather);
        signal(SIGALRM, fatheralarm);
        alarm(1);
        while(1);
    }
    else
    {
        signal(SIGUSR1, printson);
        signal(SIGUSR2, fileson);
        signal(SIGALRM, sonalarm);
        alarm(1);
        kill(getppid(), SIGUSR1);
        while(1);
    }
    return 1;
}