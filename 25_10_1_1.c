#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
 
int main(int argc, char **argv)
{
    int p = getpid();
    for (int i = 0; i != atoi(argv[1]); i++)
    {
        printf("%d\n", p);
    }
    exit(0);
}