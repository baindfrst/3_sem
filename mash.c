#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
int sg1 = 0;
int sg2 = 0;
void worker(int sig)
{
    if(sig == SIGUSR1)
    {
        sg1 += 1;
    }
    else
    {
        if (sig == SIGUSR2)
        {
            sg2 += 1;
        }
    }
}

int main()
{
    signal(SIGUSR1, worker);
    signal(SIGUSR2, worker);
    raise(SIGUSR1);
    raise(SIGUSR2);
    raise(SIGUSR1);
    raise(SIGUSR2);
    raise(SIGUSR1);
    raise(SIGUSR2);
    raise(SIGUSR1);
    printf("%d %d \n", sg1, sg2);
}