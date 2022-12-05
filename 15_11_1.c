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
        exit(2);
    }
    int file_to_print = open(argv[1], O_WRONLY | O_CREAT, 0777);
    if(file_to_print < 0)
    {
        exit(2);
    }
    pid_t pid;
    int fd_in[2];
    int fd_out[2];
    pipe(fd_in);
    pipe(fd_out);
    char prt = 's';
    char n = '\n';
    char string[81] = " ";
    char sumbol;
    pid = fork();
    char ex[] = "exit ";
    while(strcmp(string, ex) != 0)
    {
        if(pid == -1)
        {
            perror("fork");
            exit(1);
        }
        if(pid == 0)
        {
            write(fd_out[1], &prt, 1);
            read(fd_in[0], &prt, 1);
            read(fd_in[0], &sumbol, 1);
            int len = 0;
            read(fd_in[0], &len, 1);
            read(fd_in[0], string, len);
            if(strcmp(string, "exit") == 0)
            {
                exit(1);
            }
            int count = 0;
            char *out;
            for(int i = 0; i != len; i++)
            {
                if (string[i] == sumbol)
                {
                    count++;
                }
            }
            if (count > 9)
            {
                out = malloc(sizeof(char) * 3);
                out[0] = count / 10 + '0';
                out[1] = count % 10 + '0';
                out[2] = '\0';
            }
            else
            {
                out = malloc(sizeof(char) * 2);
                out[0] = count + '0';
                out[1] = '\0';
            }
            write(file_to_print, out, sizeof(char) * strlen(out));
            free(out);
            write(file_to_print, &n, sizeof(char));
        }
        else
        {
            read(fd_out[0], &prt, 1);
            read(1, &sumbol, 1);
            read(1, &prt, 1);
            char readed;
            int i = 0;
            while(read(1, &readed, 1) > 0 && readed != ' ' && readed != '\n')
            {
                string[i++] = readed;
            }
            string[i++] = ' ';
            string[i] = '\0';
            if(strcmp(string, ex) == 0)
            {
                exit(1);
            }
            write(file_to_print, string, strlen(string));
            write(fd_in[1], &prt, 1);
            write(fd_in[1], &sumbol, 1);
            i = strlen(string) - 1;
            write(fd_in[1], &i, 1);
            write(fd_in[1], string, i);
        }
    }
    close(fd_in[0]);
    close(fd_out[0]);
    close(fd_in[1]);
    close(fd_out[1]);
    close(file_to_print);
    return 0;
}