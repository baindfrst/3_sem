<<<<<<< HEAD
//S H E L L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#define N 4

void file_error(int fd);
void fork_error(pid_t pid);
void pipe_error(int p);
char* readstr();
void chek_str_and_count_com (char* str, int A[2]);
int shell();

int main()
{
    while (1)
    {
        shell();
    }

}

int shell()
{
    int i, len, n, j, k;
    int count_arg = 0, status, was_start = 0;
    int count_com_now = 0, count_com, flag_com =1;
    int flag_file_cin = 0, flag_file_cout = 0, wait_arg = 1;
    char *str;
    int A[2]; int m = 1;
    char *file1, *file2;
    int file_1, file_2;
    char **buf, **p;
    int **fd; //массив указателей на на массивы из 2х элементов 0 и 1
    pid_t pid;
    str = readstr();
    len = strlen(str);
    //delete ending spaces
    i = len-1;
    while (str[i] ==' ') i--;
    str[i+1] = '\0';
    len = strlen(str);
    chek_str_and_count_com (str, A);
    printf ("count com %d\n", A[1]);
    count_com = A[1];
    printf ("len %d\n", len);
    //выделяем память для каналов
    fd = malloc (sizeof(int*)*(count_com-1));
    for (i = 0; i <count_com-1; ++i)
    {
        fd[i] = malloc (sizeof (int)*2);
        pipe (fd[i]);
    }
    if ((str[len-1]) != '&')
    {
        //реализация без фонового режима
        //without background mode
       for (i =0; i <len; )
       {
           if (was_start)
           {
                //только что был запуск
                printf ("PETUH\n");
                //освобождение памяти
                if (was_start)
                {
                    m++;
                    for (k = 0; k <= count_arg+1; k++)
                    {
                        free(buf[k]);
                    }
                    free(buf);
                    was_start = 0;
                }
           }
           if (str[i] == ' ') i++;
           if (str[i] == '<')
           //
            {
                i++;
                while (str[i] ==' ') i++;
                //название файла
                file1 = malloc(sizeof(char));
                n = 1;
                while ((str[i] != ' ') && (i <= len - 1))
                {
                    //записываем название файла куда-то
                    file1 = realloc(file1, sizeof(char)*n);
                    file1[n-1] = str[i];
                    n++;
                    i++;
                }
                file1 = realloc(file1, sizeof(char)*n);
                file1[n-1] = '\0';
                printf ("FILEINPUT %s\n", file1);
                flag_file_cin = 1; //it was file input
                file_1 = open (file1, O_RDONLY, 0666);
            }
            if (str[i] == '>')
            {
                i++;
                printf ("TUTOS %c\nINDEX%d\n", str[i], i);
                if (str[i]=='>') i++;
                while (str[i] ==' ') i++;
                //название файла
                file2 = malloc(sizeof(char));
                n = 1;
                while ((str[i] != ' ') && (i <= len - 1))
                {
                    //записываем название файла куда-то
                    file2 = realloc(file2, sizeof(char)*n);
                    file2[n-1] = str[i];
                    n++;
                    i++;
                }
                file2 = realloc(file1, sizeof(char)*n);
                file2[n-1] = '\0';
                flag_file_cout = 1; //it was file outfile
                printf ("FILEOUTPUT %s\n", file2);
                file_2 = open (file1, O_WRONLY|O_CREAT|O_TRUNC, 0777);
            }

            if (str[i] =='|')
            {
                flag_com = 1; i++;
            }
           if (!((str[i] == ' ') || (str[i] == '<') || (str[i] == '|') || (str[i] == '<') || (str[i] == '&')))
           {
               //prosto bykva
                if (flag_com) //this - command
                {
                    count_com_now++;
                    count_arg = 0;
                    buf = malloc (sizeof(char*));
                    buf[0] = malloc (sizeof(char));
                    buf = realloc (buf, sizeof(char*)*2);
                    buf[1] = NULL;
                    n = 1;
                    while ((str[i] != ' ') && (i <= len - 1))
                    {
                        //записываем куда-то
                        buf[0] = realloc (buf[0], sizeof(char)*n);
                        buf[0][n-1] = str[i];
                        n++;
                        i++;
                    }
                    buf[0] = realloc (buf[0], sizeof(char)*n);
                    buf[0][n-1] = '\0';
                    flag_com = 0; //it was command, we wait args or '|'
                    while ((str[i] == ' ')) {i++;} //research next simvol
                    if (!((str[i] == '|') || (str[i] == '>') || (i > len-1)))
                    {
                        //то есть если дальше есть ещё аргументы
                        wait_arg = 1;

                    }
                    else
                    {
                        wait_arg = 0;
                    }
                }

                else
                {
                    //this - arg
                    while (wait_arg)
                    {
                        j = 0;
                        count_arg++;
                        buf = realloc (buf, sizeof(char*)*(count_arg+1));
                        buf[count_arg] = malloc (sizeof(char));
                        while ((str[i] != ' ') && (i <= len-1))
                        {
                            //записываем куда-то
                            buf[count_arg] = realloc (buf[count_arg], sizeof(char)*(j+2));
                            buf[count_arg][j] = str[i];
                            i++;
                            j++;
                        }
                        buf[count_arg] = realloc (buf[count_arg], sizeof(char)*(j+2));
                        buf[count_arg][j] = '\0';
                        //закончился аргумент
                        while ((str[i] == ' ')) {i++;} //research next simvol
                        //проверка на то, будут ли дальше аргументы
                        if (!((str[i] == '|') || (str[i] == '>') || (i > len-1)))
                        {
                            //то есть если дальше есть ещё аргументы
                            wait_arg = 1;

                        }
                        else
                        {
                            wait_arg = 0;
                            buf = realloc (buf, sizeof(char*)*(count_arg+2));
                            buf[count_arg+1] = NULL;
                        }

                      }
                    //мы ввели команду и её аргументы, МЫ МОЛОДЦЫ!
                  }
                //надо перенаправить ввод и вывод и запустить команду
                //запуск программы buf[0] с аргументами buf[i]

                if (!wait_arg)
                {
                    was_start = 1;
                    printf ("%s\n", buf[0]);
                    pid = fork();
                    fork_error(pid);
                    if (pid == 0)
                    {
                        //SON
                        //перенаправить ввод
                        printf ("count_com %d\ncount_com_now %d m %d\n", count_com, count_com_now, m);
                        printf ("cin %d\ncout %d\n", flag_file_cin, flag_file_cout);
                        if (count_com_now != 1) //not the first command
                        {
                            printf ("not the first command\n");
                            dup2 (fd[m-2][0], 0);
                        }

                        if ((flag_file_cin) &&(count_com_now == 1)) //use file1
                        {
                            printf ("use file1\n");
                            dup2(file_1, 0);
                        }
                        //перенаправили вывод
                        if (count_com_now != count_com) //not the last command
                        {
                            printf ("not the last command\n");
                            dup2 (fd[m-1][1], 1);
                        }
                        if ((flag_file_cout) && (count_com_now == count_com)) //use file2
                        {
                            printf ("use file2\n");
                            dup2(file_2, 1);
                        }
                            printf ("str before exe\n");
                            execvp (buf[0], buf);
                            fprintf(stderr, "failed to calling exec\n");
                            exit(1);
                    }
                    else
                    {
                        wait(&status);
                            printf ("---------------------ZAEBAl\n");
                    }
                    if (m != count_com)
                        close(fd[m-1][1]);

                }



            }

        }
        return (0);
}
    else
    {
        //НАДО ИДТИ ДО LEN - 1
        //реализация фонового режима
        //background mode
        return (0);

    }


    free(str);
    return (0);
}


char* readstr(){
    char *S = malloc(N*sizeof(char));
    int i = 0;
    int j = 1;
    int c;

    while (((c=getchar()) != '\n') && (c != EOF))
    {
        S[i] = (char)c;
        i++;
        if (N * j == i)
        {
            S = realloc(S, ((sizeof(char))*(N + N*j)));
            j++;
        }
    }
    S[i] = '\0';
    return S;

}
void file_error(int fd)
{
    if (fd == -1)
    {
         perror("file opening error");
         close (fd);
         exit (1);
    }
    return;
}

void fork_error(pid_t pid)
{
	if (pid == -1)
	{
		perror("fork error");
		kill(0, SIGKILL);
		exit(1);
	}
}

void pipe_error(int p)
{
	if (p == -1)
	{
		perror("pipe error");
		exit(1);
	}
}

void chek_str_and_count_com (char* str, int A[2])
{
    int count = 0, i, index, flag_str = 1;
    int len = strlen(str);
    for (i = 0; i< len; i++)
        {
            if (str[i] == '|') flag_str = 1; //the next str - command
            if (!((str[i] == ' ') || (str[i] == '<') || (str[i] == '|') || (str[i] == '<') || (str[i] == '&')))
            {
                //this is - start str
                if (flag_str) //this - str
                {
                    index = i; //start index of ending command
                    count++;
                }
               while ((str[i] != ' ') && (i != len-1)) {i++;} //end of word or str
               flag_str = 0; //it was command, we wait args or '|' только что была команда, ожидаем аргумента или |
            }
        }
    A[0] = index;
    A[1] = count;
    return;
}
=======
#include <stdio.h>

int main()
{
    int a = 1;
    int b = 10;
    printf("%d", !a?b:1);
}
>>>>>>> 1c09b09 (to mergen)
