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