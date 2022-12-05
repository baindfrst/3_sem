#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

void PipeError(int n){
	if (n==-1)
	{
		printf("error pipe\n");
		exit(1);
	}
}
void ForkError(pid_t p){
	if (p==-1)
	{
		printf("error fork\n");
		exit(1);
	}
}
void pingpong(char c, int cl[2], int from[2], int to[2]){
    int current, end;
    close(cl[0]);
    close(cl[1]);
	close(from[1]);
	close(to[0]);
	while (read(from[0], &current, sizeof(current))){
            read(from[0], &end, sizeof(end));
            if (current > end){
                close(from[0]);
                close(to[1]);
                if(c=='f'){
                    wait(NULL);
                }
                exit(1);
            }
            else{
                printf("%c PID:%d number %d\n",c , getpid(), current++);
                write(to[1], &current, sizeof(int));
                write(to[1], &end, sizeof(int));
            }
	}
}
int main(int argc, char** argv)
{
    pid_t p1;
    pid_t p2;
    char c;
    int n;
    int ftos[2],gstof[2],stogs[2];
    if (argc<3){
            printf("error number of arguments\n");
            exit(1);
    }
    int current=atoi(argv[1]);
    int end=atoi(argv[2]);
    if (current > end){
            printf("error arguments\n");
            exit(1);
    }
    n=pipe(ftos);
    PipeError(n);
    n=pipe(gstof);
    PipeError(n);
    write(gstof[1], &current, sizeof(int));
    write(gstof[1], &end, sizeof(int));
    p1=fork();
    ForkError(p1);

    if (p1==0){//if son
	    n=pipe(stogs);
	    PipeError(n);
	    p2=fork();
        ForkError(p2);
        printf("adad\n");
        if (p2==0){//if grandson
                c='g';
                pingpong(c, ftos, stogs, gstof);
        }
        else {
                c='s';
                pingpong(c, gstof, ftos, stogs);
        }
    }
    else {
            c='f';
            pingpong(c, stogs, gstof, ftos);
    }
    return 0;
}
