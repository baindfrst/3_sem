#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    if (argc!=3)
        return 0;
    int fd[2], status;
    pipe(fd);
    pid_t pid=fork();
    if (pid==-1)
    {
        perror("fork");
        return 0;
    }
    if (pid==0)
    {
        dup2(fd[1], 1);
        int ffrom = open(argv[1], O_RDONLY);
        if (!ffrom)
            return 0;
        int x = 0, k=1;
        while (read(ffrom, &x, sizeof(char)) > 0)
        {
            if (x!='\n')
            {
                if ((k%2)==0)
                    printf("%c", x);
            }
            else if ((k%2)==0)
            {
                printf("\n");
                k++;
            }
            else
                k++;
        }
        close(fd[0]);
        close(fd[1]);
        close(ffrom);
        exit(1);
    }
    else
    {
        wait(&status);
        close(fd[1]);
        int fto = open(argv[2], O_RDWR);
        if (!fto)
                return 0;
        dup2(fto, 1);
        dup2(fd[0], 0);
        execlp("sort", "sort", NULL);
        close(fd[0]);
        close(fto);
    }
    wait(&status);
    return 0;
}
