#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int fd[2]; char* c ="";
    int status = 0;
    pid_t pid_r, pid_w;
    pipe(fd); 
    if (pid_w =fork(), pid_w== 0)
        { 
            dup2(fd[1],1);
            close(fd[1]);
            close(fd[0]);
            execlp(argv[1],argv[1], NULL);
        }
    else 
    {
        if ((pid_r =fork())== 0)
            {
                 dup2(fd[0],0);
                 close(fd[0]);
                 close(fd[1]);
                 execlp(argv[2],argv[2], NULL);
                 exit(1);
             }
     }
     while (!WIFEXITED(status))
     {
         waitpid(pid_r, &status, WNOHANG);
     } // finished reading
     // read from pipe and printf
     dup2(fd[0],0);
     while (scanf("%s", c)>0) printf ("%s", c);
     status = 0;
     while (!WIFEXITED(status))
     {
         waitpid(pid_w, &status, WNOHANG);
     }
     
     close(fd[0]);
     close(fd[1]);

     

         return 0;
}

