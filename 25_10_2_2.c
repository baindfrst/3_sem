#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdarg.h>

void info_proc(int status)
{
    if(WIFEXITED(status))
    {
        printf("дочерний процесс завершен успешно\n");
        printf("8 младших битов, вернувшиеся из процесса: %d\n", WEXITSTATUS(status));
    }
    if(WIFSIGNALED(status))
    {
        printf("процесс завершился из за необработанного сигнала\n");
    }
}

void run_proc_par(int count, ...)//функция получает на вход кол-во аргументов и названия файла без расширения, сам файл компилируется в функции
{
    va_list ptr;
    va_start(ptr, count);
    int status;
    pid_t arr_pid[count/2];

    for(int i = 0; i != count/2; i++)
    { 
        pid_t proc;
        char* name = va_arg(ptr, char*);
        char* arg = va_arg(ptr, char*);
        if ((proc = fork()) == 0)
        {
            printf("программа %s\n ", name);
            execl(name, name, arg, NULL);
            exit(0);
        }
        else
        {
            perror("fork");
        }
        arr_pid[i] = proc;
    }
    for(int i = 0; i != count/2; i++)
    {
        waitpid(arr_pid[i], &status, 0);
        info_proc(status);
    }
    va_end(ptr);
}

int main(int argc, char **argv)
{
    int status;
    run_proc_par(argc, argv[1], argv[2], argv[3], argv[4]);
    return 0;
}