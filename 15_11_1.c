#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("error input\n");
        exit(2);
    }
    int file_to_print = open(argv[1], O_WRONLY | O_CREAT, 0777);
    if(file_to_print < 0)
    {
        printf("file error\n");
        exit(2);
    }
    pid_t pid;
    int fd_in[2];
    int fd_out[2];
    pipe(fd_in);
    pipe(fd_out);
    char prt = 's';
    char n = '\n';
    int count = 0;
    scanf("%c", &sumbol);
    gets(string);
    char string[80];
    char sumbol;
    pid = fork();
    while(strcmp(string, "exit") != 0)
    {
        write(fd_out[1], &prt, 1);
        if(pid == -1)
        {
            perror("fork");
            exit(1);
        }
        if(pid == 0)
        {
            read(fd_in[0], &prt, 1);
            for(int i = 0; i != strlen(string); i++)
            {
                if (string[i] == sumbol)
                {
                    count++;
                }
            }
            write(file_to_print, &count, 1);
            write(file_to_print, &n, sizeof(char));
            write(fd_out[1], &prt, 1);
        }
        else
        {
            read(fd_out[0], &prt, 1);
            read(1, &sumbol, 1);
            char readed;
            int i = 0;
            while(read(1, &readed, 1) > 0 && readed != ' ' && readed != '\n')
            {
                string[i++] = readed;
            }
            readed[i++] = '\0';
            write(file_to_print, string, strlen(string) + 1);
            write(fd_in[1], &prt, 1);
        }
    }
    close(fd_in[0]);
    close(fd_out[0]);
    close(fd_in[1]);
    close(fd_out[1]);
    close(file_to_print);
    return 0;
}