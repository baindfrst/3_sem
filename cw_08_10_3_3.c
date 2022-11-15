#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>



void frst(int sig)
{
    printf("pass\n");
}

int main()
{
    signal(SIGINT, frst);
    kill(0, SIGINT);
    return 0;
}