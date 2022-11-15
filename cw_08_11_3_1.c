#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>


int main()
{
    signal(SIGINT, );
    raise(SIGINT);
    return 0;
}