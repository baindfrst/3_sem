<<<<<<< HEAD
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
    pid_t pid;
    char *name = argv[1];
    int fd_in[2];
    int fd_out[2];
    pipe(fd_in);
    pipe(fd_out);
    char prt = 's';
    char n = '\n';
    int out = 0;
    int file_to_print = -1;
    pid = fork();
    while(out != 1)
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
            int len = 0;
            read(fd_in[0], &len, 1);
            if(len == 0)
            {
                exit(1);
                out = 1;
            }
            char *string = malloc(len);
            read(fd_in[0], string, len);
            if(string[len-1] == '\n')
            {
                len--;
            }
            char buf;
            for(int i = 0; i != len/2; i++)
            {
                buf = string[i];
                string[i] = string[len - i - 1];
                string[len - i - 1] = buf;
            }
            //реверс
            write(fd_out[1], &prt, 1);
            write(fd_out[1], string, len + 1);
            //отправили измененную строку
            if(string[len] != '\n') // завершаем работу после последней строчки
            {
                free(string);
                exit(1);
                out = 1;
            }
            free(string);
        }
        else
        {
            read(fd_out[0], &prt, 1);
            if(file_to_print == -1)
            {
                file_to_print = open(name, O_RDWR | O_CREAT, 0777);
                if(file_to_print < 0)
                {
                    exit(2);
                }
            }
            //открыли файл
            char *string;
            int ch;
            char readed;
            int len = 0;
            int go_to_write;
            go_to_write =  lseek(file_to_print, 0, SEEK_CUR);
            while((ch = read(file_to_print, &readed, 1)) > 0 && readed != '\n')
            {
                if(len == 0)
                {
                    string = malloc(sizeof(char));
                    string[len] = readed;
                }
                else
                {
                    string = realloc(string, sizeof(char) * (len + 1));
                    string[len] = readed;
                }
                len++;
            }
            if(ch != 0)
            {
                if (readed == '\n')
                {
                    if(len == 0)
                    {
                        string = malloc(sizeof(char));
                        string[len] = readed;
                    }
                    else
                    {
                        string = realloc(string, sizeof(char) * (len + 1));
                        string[len] = readed;
                    }
                    len++;
                }
            }
            write(fd_in[1], &prt, 1);
            write(fd_in[1], &len, 1);
            write(fd_in[1], string, len);
            if(len == 0)
            {
                exit(1);
                out = 1;
            }
            //после ожидания реверса
            read(fd_out[0], &prt, 1);
            free(string);
            string = malloc(sizeof(char) * len);
            read(fd_out[0], string, len); //прочли измен строчку
            lseek(file_to_print, go_to_write, SEEK_SET);
            write(file_to_print, string, len);
            if(string[len-1] != '\n')
            {
                free(string);
                exit(1);
                out = 1;
            }
            free(string);
        }
    }
    close(fd_in[0]);
    close(fd_out[0]);
    close(fd_in[1]);
    close(fd_out[1]);
    close(file_to_print);
    return 0;
=======
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
    pid_t pid;
    char *name = argv[1];
    int fd_in[2];
    int fd_out[2];
    pipe(fd_in);
    pipe(fd_out);
    char prt = 's';
    char n = '\n';
    int out = 0;
    int file_to_print = -1;
    pid = fork();
    while(out != 1)
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
            int len = 0;
            read(fd_in[0], &len, 1);
            if(len == 0)
            {
                exit(1);
                out = 1;
            }
            char *string = malloc(len);
            read(fd_in[0], string, len);
            if(string[len-1] == '\n')
            {
                len--;
            }
            char buf;
            for(int i = 0; i != len/2; i++)
            {
                buf = string[i];
                string[i] = string[len - i - 1];
                string[len - i - 1] = buf;
            }
            //реверс
            write(fd_out[1], &prt, 1);
            write(fd_out[1], string, len + 1);
            //отправили измененную строку
            if(string[len] != '\n') // завершаем работу после последней строчки
            {
                free(string);
                exit(1);
                out = 1;
            }
            free(string);
        }
        else
        {
            read(fd_out[0], &prt, 1);
            if(file_to_print == -1)
            {
                file_to_print = open(name, O_RDWR | O_CREAT, 0777);
                if(file_to_print < 0)
                {
                    exit(2);
                }
            }
            //открыли файл
            char *string;
            int ch;
            char readed;
            int len = 0;
            int go_to_write;
            go_to_write =  lseek(file_to_print, 0, SEEK_CUR);
            while((ch = read(file_to_print, &readed, 1)) > 0 && readed != '\n')
            {
                if(len == 0)
                {
                    string = malloc(sizeof(char));
                    string[len] = readed;
                }
                else
                {
                    string = realloc(string, sizeof(char) * (len + 1));
                    string[len] = readed;
                }
                len++;
            }
            if(ch != 0)
            {
                if (readed == '\n')
                {
                    if(len == 0)
                    {
                        string = malloc(sizeof(char));
                        string[len] = readed;
                    }
                    else
                    {
                        string = realloc(string, sizeof(char) * (len + 1));
                        string[len] = readed;
                    }
                    len++;
                }
            }
            write(fd_in[1], &prt, 1);
            write(fd_in[1], &len, 1);
            write(fd_in[1], string, len);
            if(len == 0)
            {
                exit(1);
                out = 1;
            }
            //после ожидания реверса
            read(fd_out[0], &prt, 1);
            free(string);
            string = malloc(sizeof(char) * len);
            read(fd_out[0], string, len); //прочли измен строчку
            lseek(file_to_print, go_to_write, SEEK_SET);
            write(file_to_print, string, len);
            if(string[len-1] != '\n')
            {
                free(string);
                exit(1);
                out = 1;
            }
            free(string);
        }
    }
    close(fd_in[0]);
    close(fd_out[0]);
    close(fd_in[1]);
    close(fd_out[1]);
    close(file_to_print);
    return 0;
>>>>>>> 1c09b09 (to mergen)
}