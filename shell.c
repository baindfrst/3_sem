<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
int input_file, output_file, count_args, unwwait;
int input_bg_file = -1;
int out_bg_file = -1;
int background_count = 0;
int **fd;
int pid_fin;

char* reade_string(char* string)
{
    short malloced = 0;
    int size_of_string = 0;
    char readed;
    while((readed = getchar()) != EOF && readed != '\n')
    {
        if(malloced)
        {
            string = realloc(string, (size_of_string + 1) * sizeof(char));
            string[size_of_string++] = readed;
        }
        else
        {
            string = malloc(sizeof(char));
            string[size_of_string++] = readed;
            malloced++;
        }
    }
    if(malloced)
    {
        string = realloc(string, (size_of_string + 1) * sizeof(char));
        string[size_of_string] = '\0';
    }
    else
    {
        string = malloc(sizeof(char));
        string[size_of_string] = '\0';
        malloced++;
    }
    return string;
}

short input_file_param(char* string, char** file_name)
{
    short flag = 0;
    if(string[0] == '<')
    {
        flag++;
        short malloced = 0;
        int i = 1;
        while(string[i] != ' ')
        {
            if(malloced)
            {
                *file_name = realloc(*file_name, i * sizeof(char));
                (*file_name)[i - 1] = string[i];
            }
            else
            {
                *file_name = malloc(sizeof(char));
                (*file_name)[i - 1] = string[i];
                malloced++;
            }
            i++;
        }
        if(malloced)
        {
            *file_name = realloc(*file_name, i * sizeof(char));
            (*file_name)[i++ - 1] = '\0';
        }
        else
        {
            *file_name = malloc(sizeof(char));
            (*file_name)[i++ - 1] = '\0';
            malloced++;
        }
    }
    return flag;
}

short output_file_param(char* string, char** file_name)
{
    short flag = 0;
    int i = strlen(string) - 1;
    while(string[i] != ' ')
    {
        i--;
    }
    i++;
    if(string[i] == '>')
    {
        flag++;
        i++;
        if(string[i] == '>')
        {
            flag++;
            i++;
        }
        short malloced = 0;
        int len = 0;
        while(string[i] != '\0')
        {
            if(malloced)
            {
                *file_name = realloc(*file_name, (len + 1) * sizeof(char));
                (*file_name)[len++] = string[i];
            }
            else
            {
                *file_name = malloc(sizeof(char));
                (*file_name)[len++] = string[i];
                malloced++;
            }
            i++;
        }
        if(malloced)
        {
            *file_name = realloc(*file_name, (len + 1) * sizeof(char));
            (*file_name)[len] = '\0';
        }
        else
        {
            *file_name = malloc(sizeof(char));
            (*file_name)[len] = '\0';
            malloced++;
        }
    }
    return flag;
}

int get_args_list(char* string, char***** args_list)
{
    int i = 0;
    int count = 0;
    if (string[i] == '<')
    {
        while(string[i] != ' ')
        {
            i++;
        }
    }
    int flag_move = 0;
    while(string[i] != '\0' && string[i] != '>')
    {
        if(flag_move)
            {
                i++;
            }
        while(string[i] == ' ')
        {
            i++;
        }
        if(count == 0)
        {
            (*args_list) = malloc(sizeof(char****));
        }
        else
        {
            (*args_list) = realloc((*args_list), (count + 1) * sizeof(char****));
        }
        (*args_list)[count] = malloc(sizeof(char***) + sizeof(char***));
        (*args_list)[count][0] = malloc(sizeof(char**));
        (*args_list)[count][0][0] = malloc(sizeof(char));
        (*args_list)[count][0][0][0] = 'N';
        
        int count_commands = 0;
        while(string[i] != '\0' && string[i] != '>' && string[i] != '|')
        {
            if(count_commands == 0)
            {
                (*args_list)[count][1] = malloc(sizeof(char*));
            }
            else
            {
                (*args_list)[count][1] = realloc((*args_list)[count][1], sizeof(char*) * (count_commands + 1));
            }
            int len_command = 0;
            short malloced = 0;
            while(string[i] != ' ' && string[i] != '\0' && string[i] != '>' && string[i] != '|')
            {
                if(malloced)
                {
                    (*args_list)[count][1][count_commands] = realloc((*args_list)[count][1][count_commands], (len_command + 1) * sizeof(char));
                    (*args_list)[count][1][count_commands][len_command++] = string[i];
                }
                else
                {
                    (*args_list)[count][1][count_commands] = malloc(sizeof(char));
                    (*args_list)[count][1][count_commands][len_command++] = string[i];
                    malloced++;
                }
                i++;
            }
            if(malloced)
            {
                (*args_list)[count][1][count_commands] = realloc((*args_list)[count][1][count_commands], (len_command + 1) * sizeof(char));
                (*args_list)[count][1][count_commands][len_command++] = '\0';
            }
            else
            {
                (*args_list)[count][1][count_commands] = malloc(sizeof(char));
                (*args_list)[count][1][count_commands][len_command++] = '\0';
                malloced++;
            }
            if(string[i] == ' ')
            {
                while(string[i] == ' ')
                {
                    i++;
                }
            }
            if(string[i] != '\0' && string[i] != '>' && string[i] != '|')
            {
                count_commands++;
            }
            else
            {
                if(strlen((*args_list)[count][1][count_commands]) == 1 && (*args_list)[count][1][count_commands][0] == '&')
                {
                    (*args_list)[count][0][0][0] = '&';
                    free((*args_list)[count][1][count_commands]);
                    count_commands--;
                }
                (*args_list)[count][1] = realloc((*args_list)[count][1], sizeof(char*) * (count_commands + 1) + sizeof(NULL));
                (*args_list)[count][1][++count_commands] = NULL;
            }
            if(string[i] == '|')
            {
                flag_move = 1;
            }
            else
            {
                flag_move = 0;
            }
        }
        count++;
    } 
    return count;
}

void malloc_pipes()
{
    fd=(int **)malloc((count_args-1)*sizeof(int *));
	if (!fd)
	{
		fprintf(stderr, "Failed to allocate memory \n");
		exit(1); 
	}
	for(int i=0;i<count_args-1;i++)
	{
		fd[i]=(int *)malloc(2*sizeof(int));
		if (!fd[i])
		{
			fprintf(stderr, "Failed to allocate memory \n");
			exit(1); 
		}
	}
}

void open_pipes()
{
    int p;
    for (int i = 0; i != count_args - 1; i++)
    {
        p = pipe(fd[i]);
        if(p < 0)
        {
            perror("error of open pipe");
            exit(-1);
        }
    }
}

void waiting(int f)
{
    if(f)
    {
        wait(&unwwait);
    }
    else
    {
        for(int i = 0; i != background_count; i++)
        {
            wait(&unwwait);
        }
    }
}

void close_pipes()
{
    for(int i=0;i<count_args-1;i++)
	{
		close(fd[i][0]);
		close(fd[i][1]);
	}	
}

void free_arr_of_args(char***** args_list, int len)
{
    for(int i = 0; i != len; i++)
    {
        free((*args_list)[i][0][0]);
        free((*args_list)[i][0]);
        int j = 0;
        while((*args_list)[i][1][j] != NULL)
        {
            free((*args_list)[i][1][j++]);
        }
        free((*args_list)[i][1]);
        free((*args_list)[i]);
    }
    free(*args_list);
}

void free_fd()
{
    for(int i = 0; i != count_args - 1; i++)
    {
        free(fd[i]);
    }
    free(fd);
}

int main(int argc, char** argv)
{
    char**** args; // [[[['&/n']], [com, arg1, arg2 ...]], [[['&/n']], [com, arg1, arg2 ...]] ...]
    char* command_line;
    command_line = reade_string(command_line);
    char* input_file_name;
    char* output_file_name;
    count_args = get_args_list(command_line, &args);
    int input_file_flag = 0;
    input_file_flag = input_file_param(command_line, &input_file_name);
    short output_file_flag = output_file_param(command_line, &output_file_name);
    malloc_pipes();
    open_pipes();
    for(int i = 0; i != count_args; i++)
    {
        if(args[i][0][0][0] != '&')
        {
            int pid_fin = fork();
            if (pid_fin == -1)
            {
                perror("fork error");
                exit(-1);
            }
            if(pid_fin == 0)
            {
                if(i == 0 && input_file_flag) //input block
                {
                    if((input_file = open(input_file_name, O_RDONLY|O_CREAT)) < 0)
                    {
                        perror("cannot open input file");
                        exit(1);
                    }
                    if(dup2(input_file, 0) < 0)
                    {
                        perror("dup2 input from file error");
                        exit(-1);
                    }
                }
                else
                {
                    if(i != 0)
                    {
                        if(dup2(fd[i-1 - background_count][0], 0) < 0)
                        {
                            perror("dup2 input from pipe error");
                            exit(-1);
                        }
                    }
                }
                if((i == count_args - 1) && output_file_flag) //out block
                {
                    if(output_file_flag == 1)
                    {
                        if((output_file = open(output_file_name, O_WRONLY|O_CREAT)) < 0)
                        {
                            perror("cannot open output file");
                            exit(1);
                        }
                    }
                    else
                    {
                        if(output_file_flag == 2)
                        {
                            if((output_file = open(output_file_name, O_WRONLY | O_APPEND | O_CREAT)) < 0)
                            {
                                perror("cannot open output file");
                                exit(1);
                            }
                        }
                    }
                    if(dup2(output_file, 1) < 0)
                    {
                        perror("dup2 output to file error");
                        exit(-1);
                    }
                }
                else
                {
                    if(i != count_args - 1)
                    {
                        if(dup2(fd[i - background_count][1], 1) < 0)
                        {
                            perror("dup2 output to pipe error");
                            exit(-1);
                        }
                    }
                }
                close_pipes();
                execvp(args[i][1][0], args[i][1]);
                fprintf(stderr, "failed to calling exec\n");
                exit(1);
            }
        }
        else //background procces
        {
            int p = fork();
            background_count++;
            if (p == -1)
            {
                perror("fork error");
                exit(-1);
            }
            if(p == 0)
            {
                p = fork();
                if (p == -1)
                {
                    perror("fork error");
                    exit(-1);
                }
                if(p == 0)
                {
                    kill(getppid(), SIGINT);
                    signal(SIGINT, SIG_IGN);
                    if(input_file_flag) //input block
                    {
                        if(dup2(input_file, 0) < 0)
                        {
                            perror("dup2 input from file error");
                            exit(-1);
                        }
                    }
                    else
                    {
                        input_bg_file = open("/dev/null", O_RDONLY, 0666);
                        if(dup2(input_bg_file, 0) < 0)
                        {
                            perror("dup2 input from pipe error");
                            exit(-1);
                        }
                    }
                    if(output_file_flag) //out block
                    {
                        if(dup2(output_file, 1) < 0)
                        {
                            perror("dup2 output to file error");
                            exit(-1);
                        }
                    }
                    else
                    {
                        out_bg_file = open("/dev/null", O_WRONLY, 0666);
                        if(dup2(out_bg_file, 1) < 0)
                        {
                            perror("dup2 output to pipe error");
                            exit(-1);
                        }
                    }
                    close_pipes();
                    execvp(args[i][1][0], args[i][1]);
                    fprintf(stderr, "failed to calling exec\n");
                    exit(1);
                }
            }
        }
    }
    close_pipes();
    waitpid(pid_fin, &unwwait);
    free_arr_of_args(&args, count_args);
    free_fd();
    free(command_line);
    if(input_file_flag)
    {
        free(input_file_name);
    }
    if(output_file_flag)
    {
        free(output_file_name);
    }
    if(input_bg_file > -1)
    {
        close(input_bg_file);
    }
    if(out_bg_file > -1)
    {
        close(out_bg_file);
    }
}
=======
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
    int index;
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
    index = len -1;
    printf ("len %d\n", len);
    //есть ли перенаправление вывода в файл?
    for  (i = 0; i< len; ++i)
    {
        if (str[i] == '>')
            {
                index = i -1;
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
                file2 = realloc(file2, sizeof(char)*n);
                file2[n-1] = '\0';
                flag_file_cout = 1; //it was file outfile
                printf ("FILEOUTPUT %s\n", file2);
                if ((file_2 = open (file2, O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0)
                {
                    perror("cannot open output file\n");
                    exit(1);
                }
                break;
            }
    }
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
       for (i =0; i <=index; )
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
           if (str[i] == '>')
           {
                i++;
           }
           if (str[i] == '<')
           //
            {
                i++;
                while (str[i] ==' ') i++;
                //название файла
                file1 = malloc(sizeof(char));
                n = 1;
                while ((str[i] != ' ') && (i <= index))
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
                file_1 = open (file1, O_RDONLY, 0666); //< 0)
                /*{
                    perror("cannot open input file\n");
                    exit(1);
                }*/

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
                    while ((str[i] != ' ') && (i <= index))
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
                    if (!((str[i] == '|') || (str[i] == '>') || (i > index)))
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
                        while ((str[i] != ' ') && (i <= index))
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
                        if (!((str[i] == '|') || (str[i] == '>') || (i > index)))
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
                            dup2 (fd[m-2][0], 0); //fprintf(stderr, "not the first com error\n");
                        }

                        if ((count_com_now == 1) && (flag_file_cin)) //use file1 and first com
                        {
                            printf ("use file1\n");
                            dup2(file_1, 0); //fprintf(stderr, "f1 error\n");
                        }
                        //перенаправили вывод
                        if (count_com_now != count_com) //not the last command
                        {
                            printf ("not the last command\n");
                            dup2 (fd[m-1][1], 1);
                        }
                        printf ("TUT1\n");
                        if ((count_com_now == count_com) && (flag_file_cout)) //use file2 and last com
                        {
                            printf ("use file2\n");
                            dup2(file_2, 1); //fprintf(stderr, "f2 error\n");
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
>>>>>>> 1c09b09 (to mergen)
