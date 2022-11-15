#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

void scnd(int sig)
{
    printf("SIGINT\n");
}

void thrd(int sig)
{
    printf("SIGILL\n");
}


void frst(int sig)
{
    raise(SIGILL);
    signal(SIGINT, scnd);
    raise(SIGINT);
}

int main()
{
    signal(SIGINT, frst);
    signal(SIGILL, thrd);
    raise(SIGINT);
    return 0;
}