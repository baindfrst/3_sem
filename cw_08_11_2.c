#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

static int wait = 0;
static int count = 0;

void work_sig(int sig)
{
    if(wait >= 2 && wait <= 4)
    {
        count++;
    }
    getchar();
}

void listener(int sig) 
{
    wait++;
    if(wait >= 2 && wait <= 4)
    {
        signal(SIGTSTP, work_sig);
    }
    else
    {
        if(wait > 4)
        {
            signal(SIGTSTP, SIG_IGN);
        }
    }
    if(wait == 6)
    {
        printf("\n SIGTSTP %d\n", count);
        signal(SIGINT, SIG_DFL);
        exit(1);
    }
    getchar();
}

void main() 
{
    signal(SIGINT, listener);
    signal(SIGTSTP, SIG_IGN);

    do {
    } while (wait != 6);
    getchar();
}