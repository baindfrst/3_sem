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

int count_of_sim(char* string, char simb)
{
    int count = 0;
    for (int i = 0; string[i] != '\0'; i++)
    {
        if(string[i] == simb)
        {
            count++;
        }
    }
    return count;
}

int check_input_data(char* string)
{
    int start = 0;
    int stop = strlen(string) - 1;
    if(count_of_sim(string, '&') > 1)
    {
        fprintf(stderr, "& error\n");
        exit(1);
    }
    else
    {
        if(count_of_sim(string, '&') == 1)
        {
            int i = strlen(string) - 1;
            while(string[i] == ' ')
            {
                i--;
            }
            if(string[i] != '&')
            {
                fprintf(stderr, "& error pos\n");
                exit(1);
            }
        }
    }
    if(count_of_sim(string, '<') > 1)
    {
        fprintf(stderr, "input file count error\n");
        exit(1);
    }
    else
    {
        if(count_of_sim(string, '<') == 1)
        {
            int i = 0;
            while(string[i] != '<')
            {
                if(string[i] != ' ')
                {
                    fprintf(stderr, "input file pos err\n");
                    exit(1);
                }
                i++;
            }
            while(string[i] != ' ')
            {
                i++;
            }
            start = i;
        }
    }
    if(count_of_sim(string, '>') > 2)
    {
        fprintf(stderr, "output file count error\n");
        exit(1);
    }
    else
    {
        if(count_of_sim(string, '>') == 1 && count_of_sim(string, '>') == 2)
        {
            int i = strlen(string) - 1;
            int j = strlen(string) - 1;
            while(string[j] != '>')
            {
                j--;
            }
            while(string[j] != ' ' && string[j] != '\0')
            {
                j++;
            }
            if(string[j] == '\0')
            {
                j--;
            }
            while(i != j)
            {
                if(string[i] != ' ' && string[i] != '&')
                {
                    fprintf(stderr, "output file pos err\n");
                    exit(1);
                }
                i--;
            }
        }
    }
    int was_simb = 0;
    int com_was = 0;
    while(start != stop)
    {
        while(string[start] != '|' && start != stop)
        {
            if(string[start] != ' ')
            {
                was_simb++;
                com_was++;
            }
            start++;
        }
        if(was_simb == 0)
        {
            fprintf(stderr, "commands count error\n");
            exit(1);
        }
        if(string[start] == '|')
        {
            start++;
        }
    }
    if(com_was == 0)
    {
        fprintf(stderr, "No commands\n");
        exit(1);
    }
}

short input_file_param(char* string, char** file_name)
{
    short flag = 0;
    int i = 0;
    while(string[i] == ' ')
    {
        i++;
    }
    if(string[i] == '<')
    {
        i++;
        int j = 0;
        flag++;
        short malloced = 0;
        while(string[i] != ' ')
        {
            if(malloced)
            {
                *file_name = realloc(*file_name, i * sizeof(char));
                (*file_name)[j++] = string[i];
            }
            else
            {
                *file_name = malloc(sizeof(char));
                (*file_name)[j++] = string[i];
                malloced++;
            }
            i++;
        }
        if(malloced)
        {
            *file_name = realloc(*file_name, i * sizeof(char));
            (*file_name)[j++] = '\0';
        }
        else
        {
            *file_name = malloc(sizeof(char));
            (*file_name)[j++] = '\0';
            malloced++;
        }
    }
    return flag;
}

short output_file_param(char* string, char** file_name)
{
    short flag = 0;
    int i = strlen(string) - 1;
    while(string[i] == ' ' && i != 0)
    {
        i--;
    }
    if(string[i] == '&' && i != 0)
    {
        i--;
    }
    while(string[i] == ' ' && i != 0)
    {
        i--;
    }
    while(string[i] != ' ' && i != 0)
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
        while(string[i] != '\0' && string[i] != ' ')
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

int get_args_list(char* string, char**** args_list)
{
    int i = 0;
    int count = 0;
    while(string[i] == ' ')
    {
        i++;
    }
    if (string[i] == '<')
    {
        while(string[i] != ' ')
        {
            i++;
        }
    }
    int flag_move = 0;
    while(string[i] != '\0' && string[i] != '>' && string[i] != EOF && string[i] != '&')
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
            (*args_list) = malloc(sizeof(char***));
        }
        else
        {
            (*args_list) = realloc((*args_list), (count + 1) * sizeof(char***));
        }
        int count_commands = 0;
        while(string[i] != '\0' && string[i] != '>' && string[i] != '|' && string[i] != '&')
        {
            if(count_commands == 0)
            {
                (*args_list)[count] = malloc(sizeof(char*));
            }
            else
            {
                (*args_list)[count] = realloc((*args_list)[count], sizeof(char*) * (count_commands + 1));
            }
            int len_command = 0;
            short malloced = 0;
            while(string[i] != ' ' && string[i] != '\0' && string[i] != '>' && string[i] != '|' && string[i] != '&')
            {
                if(malloced)
                {
                    (*args_list)[count][count_commands] = realloc((*args_list)[count][count_commands], (len_command + 1) * sizeof(char));
                    (*args_list)[count][count_commands][len_command++] = string[i];
                }
                else
                {
                    (*args_list)[count][count_commands] = malloc(sizeof(char));
                    (*args_list)[count][count_commands][len_command++] = string[i];
                    malloced++;
                }
                i++;
            }
            if(malloced)
            {
                (*args_list)[count][count_commands] = realloc((*args_list)[count][count_commands], (len_command + 1) * sizeof(char));
                (*args_list)[count][count_commands][len_command++] = '\0';
            }
            else
            {
                (*args_list)[count][count_commands] = malloc(sizeof(char));
                (*args_list)[count][count_commands][len_command++] = '\0';
                malloced++;
            }
            if(string[i] == ' ')
            {
                while(string[i] == ' ')
                {
                    i++;
                }
            }
            if(string[i] != '\0' && string[i] != '>' && string[i] != '|' && string[i] != '&')
            {
                count_commands++;
            }
            else
            {
                (*args_list)[count] = realloc((*args_list)[count], sizeof(char*) * (count_commands + 1) + sizeof(NULL));
                (*args_list)[count][++count_commands] = NULL;
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

int check_background_work(char* string)
{
    int i = strlen(string) - 1;
    while(string[i] == ' ')
    {
        i--;
    }
    if(string[i] == '&')
    {
        return 1;
    }
    else
    {
        return 0;
    }
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

void close_pipes()
{
    for(int i=0;i<count_args-1;i++)
	{
		close(fd[i][0]);
		close(fd[i][1]);
	}
}

void free_arr_of_args(char**** args_list, int len)
{
    for(int i = 0; i != len; i++)
    {
        int j = 0;
        while((*args_list)[i][j] != NULL)
        {
            free((*args_list)[i][j++]);
        }
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
    char*** args; // [[[com, arg1, arg2 ...]], [[['&/n']], [com, arg1, arg2 ...]] ...]
    char* command_line;
    char* input_file_name;
    char* output_file_name;
    short output_file_flag;
    int input_file_flag;
    char out;
    short flag_nonstop = 1;
    while(flag_nonstop)
    {
        command_line = reade_string(command_line);
        if(strlen(command_line) == 0)
        {
            fprintf(stderr, "No commands\n");
            exit(1);
        }
        check_input_data(command_line);
        count_args = get_args_list(command_line, &args);
        input_file_flag = input_file_param(command_line, &input_file_name);
        output_file_flag = output_file_param(command_line, &output_file_name);
        if(count_args == 0)
        {
            fprintf(stderr, "No commands\n");
            exit(1);
        }
        malloc_pipes();
        if(check_background_work(command_line) == 0)
        {
            open_pipes();
            for(int i = 0; i != count_args; i++)
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
                        if((input_file = open(input_file_name, O_RDONLY)) < 0)
                        {
                            perror("cannot open input file");
                            exit(1);
                        }
                        if(  (input_file, 0) < 0)
                        {
                            perror("dup2 input from file error");
                            exit(-1);
                        }
                    }
                    else
                    {
                        if(i != 0)
                        {
                            if(dup2(fd[i-1][0], 0) < 0)
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
                            if((output_file = open(output_file_name, O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0)
                            {
                                perror("cannot open output file");
                                exit(1);
                            }
                        }
                        else
                        {
                            if(output_file_flag == 2)
                            {
                                if((output_file = open(output_file_name, O_WRONLY | O_APPEND | O_CREAT, 0777)) < 0)
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
                            if(dup2(fd[i][1], 1) < 0)
                            {
                                perror("dup2 output to pipe error");
                                exit(-1);
                            }
                        }
                    }
                    close_pipes();
                    execvp(args[i][0], args[i]);
                    fprintf(stderr, "failed to calling exec\n");
                    exit(1);
                }
            }
            for(int i = 0; i != count_args - 1; i++)
                {
                    wait(&unwwait);
                }
        }
        else //background procces
        {
            open_pipes();
            for(int i = 0; i != count_args; i++)
            {
                int p = fork();
                if (p == -1)
                {
                    perror("fork error");
                    exit(-1);
                }
                if(p == 0)
                {
                    signal(SIGINT, SIG_IGN);
                    p = fork();
                    if (p == -1)
                    {
                        perror("fork error");
                        exit(-1);
                    }
                    if(p == 0)
                    {
                        signal(SIGINT, SIG_IGN);
                        if(input_file_flag && (i == 0)) //input block
                        {
                            if((input_file = open(input_file_name, O_RDONLY)) < 0)
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
                            if(i == 0)
                            {
                                if((input_bg_file = open("/dev/null", O_RDONLY, 0666)) < 0 )
                                {
                                    perror("input NULL oprn error");
                                    exit(-1);
                                }
                                if(dup2(input_bg_file, 0) < 0)
                                {
                                    perror("dup2 input from pipe error");
                                    exit(-1);
                                }
                            }
                            else
                            {
                                if(dup2(fd[i-1][0], 0) < 0)
                                {
                                    perror("dup2 input from pipe error");
                                    exit(-1);
                                }
                            }
                        }
                        if(output_file_flag && (i == count_args - 1)) //out block
                        {
                            if(output_file_flag == 1)
                            {
                                if((output_file = open(output_file_name, O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0)
                                {
                                    perror("cannot open output file");
                                    exit(1);
                                }
                            }
                            else
                            {
                                if(output_file_flag == 2)
                                {
                                    if((output_file = open(output_file_name, O_WRONLY | O_APPEND | O_CREAT, 0777)) < 0)
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
                            if(i == count_args - 1)
                            {
                                if((out_bg_file = open("/dev/null", O_WRONLY, 0666)) < 0)
                                {
                                    perror("output NULL open error");
                                    exit(-1);
                                }
                                if(dup2(out_bg_file, 1) < 0)
                                {
                                    perror("dup2 output to pipe error");
                                    exit(-1);
                                }
                            }
                            else
                            {
                                if(dup2(fd[i][1], 1) < 0)
                                {
                                    perror("dup2 output from pipe error");
                                    exit(-1);
                                }
                            }
                        }
                        close_pipes();
                        execvp(args[i][0], args[i]);
                        fprintf(stderr, "failed to calling exec\n");
                        exit(1);
                    }
                    exit(0);
                }
                for(int i = 0; i != count_args; i++)
                {
                    wait(&unwwait);
                }
            }
        }
        close_pipes();
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
        sleep(1);
        printf("continue [Y/N]:\n");
        out = getchar();
        getchar();
        if(out == 'n' | out == 'N')
        {
            flag_nonstop = 0;
        }
        else
        {
            flag_nonstop = 1;
        }
    }
    exit(0);
}
=======
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

int count_of_sim(char* string, char simb)
{
    int count = 0;
    for (int i = 0; string[i] != '\0'; i++)
    {
        if(string[i] == simb)
        {
            count++;
        }
    }
    return count;
}

int check_input_data(char* string)
{
    int start = 0;
    int stop = strlen(string) - 1;
    if(count_of_sim(string, '&') > 1)
    {
        fprintf(stderr, "& error\n");
        exit(1);
    }
    else
    {
        if(count_of_sim(string, '&') == 1)
        {
            int i = strlen(string) - 1;
            while(string[i] == ' ')
            {
                i--;
            }
            if(string[i] != '&')
            {
                fprintf(stderr, "& error pos\n");
                exit(1);
            }
        }
    }
    if(count_of_sim(string, '<') > 1)
    {
        fprintf(stderr, "input file count error\n");
        exit(1);
    }
    else
    {
        if(count_of_sim(string, '<') == 1)
        {
            int i = 0;
            while(string[i] != '<')
            {
                if(string[i] != ' ')
                {
                    fprintf(stderr, "input file pos err\n");
                    exit(1);
                }
                i++;
            }
            while(string[i] != ' ')
            {
                i++;
            }
            start = i;
        }
    }
    if(count_of_sim(string, '>') > 2)
    {
        fprintf(stderr, "output file count error\n");
        exit(1);
    }
    else
    {
        if(count_of_sim(string, '>') == 1 && count_of_sim(string, '>') == 2)
        {
            int i = strlen(string) - 1;
            int j = strlen(string) - 1;
            while(string[j] != '>')
            {
                j--;
            }
            stop = j;
            while(string[stop] != ' ')
            {
                stop--;
            }
            while(string[j] != ' ')
            {
                j++;
            }
            while(i != j)
            {
                if(string[i] != ' ' && string[i] != '&')
                {
                    fprintf(stderr, "output file pos err\n");
                    exit(1);
                }
                i--;
            }
        }
    }
    int was_simb = 0;
    int com_was = 0;
    while(start != stop)
    {
        while(string[start] != '|' && start != stop)
        {
            if(string[start] != ' ')
            {
                was_simb++;
                com_was++;
            }
            start++;
        }
        if(was_simb == 0)
        {
            fprintf(stderr, "commands count error\n");
            exit(1);
        }
        if(string[start] == '|')
        {
            start++;
        }
    }
    if(com_was == 0)
    {
        fprintf(stderr, "No commands\n");
        exit(1);
    }
}

short input_file_param(char* string, char** file_name)
{
    short flag = 0;
    int i = 0;
    while(string[i] == ' ')
    {
        i++;
    }
    if(string[i] == '<')
    {
        i++;
        int j = 0;
        flag++;
        short malloced = 0;
        while(string[i] != ' ')
        {
            if(malloced)
            {
                *file_name = realloc(*file_name, i * sizeof(char));
                (*file_name)[j++] = string[i];
            }
            else
            {
                *file_name = malloc(sizeof(char));
                (*file_name)[j++] = string[i];
                malloced++;
            }
            i++;
        }
        if(malloced)
        {
            *file_name = realloc(*file_name, i * sizeof(char));
            (*file_name)[j++] = '\0';
        }
        else
        {
            *file_name = malloc(sizeof(char));
            (*file_name)[j++] = '\0';
            malloced++;
        }
    }
    return flag;
}

short output_file_param(char* string, char** file_name)
{
    short flag = 0;
    int i = strlen(string) - 1;
    while(string[i] == ' ' && i != 0)
    {
        i--;
    }
    if(string[i] == '&' && i != 0)
    {
        i--;
    }
    while(string[i] == ' ' && i != 0)
    {
        i--;
    }
    while(string[i] != ' ' && i != 0)
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
        while(string[i] != '\0' && string[i] != ' ')
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

int get_args_list(char* string, char**** args_list)
{
    int i = 0;
    int count = 0;
    while(string[i] == ' ')
    {
        i++;
    }
    if (string[i] == '<')
    {
        while(string[i] != ' ')
        {
            i++;
        }
    }
    int flag_move = 0;
    while(string[i] != '\0' && string[i] != '>' && string[i] != EOF && string[i] != '&')
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
            (*args_list) = malloc(sizeof(char***));
        }
        else
        {
            (*args_list) = realloc((*args_list), (count + 1) * sizeof(char***));
        }
        int count_commands = 0;
        while(string[i] != '\0' && string[i] != '>' && string[i] != '|' && string[i] != '&')
        {
            if(count_commands == 0)
            {
                (*args_list)[count] = malloc(sizeof(char*));
            }
            else
            {
                (*args_list)[count] = realloc((*args_list)[count], sizeof(char*) * (count_commands + 1));
            }
            int len_command = 0;
            short malloced = 0;
            while(string[i] != ' ' && string[i] != '\0' && string[i] != '>' && string[i] != '|' && string[i] != '&')
            {
                if(malloced)
                {
                    (*args_list)[count][count_commands] = realloc((*args_list)[count][count_commands], (len_command + 1) * sizeof(char));
                    (*args_list)[count][count_commands][len_command++] = string[i];
                }
                else
                {
                    (*args_list)[count][count_commands] = malloc(sizeof(char));
                    (*args_list)[count][count_commands][len_command++] = string[i];
                    malloced++;
                }
                i++;
            }
            if(malloced)
            {
                (*args_list)[count][count_commands] = realloc((*args_list)[count][count_commands], (len_command + 1) * sizeof(char));
                (*args_list)[count][count_commands][len_command++] = '\0';
            }
            else
            {
                (*args_list)[count][count_commands] = malloc(sizeof(char));
                (*args_list)[count][count_commands][len_command++] = '\0';
                malloced++;
            }
            if(string[i] == ' ')
            {
                while(string[i] == ' ')
                {
                    i++;
                }
            }
            if(string[i] != '\0' && string[i] != '>' && string[i] != '|' && string[i] != '&')
            {
                count_commands++;
            }
            else
            {
                (*args_list)[count] = realloc((*args_list)[count], sizeof(char*) * (count_commands + 1) + sizeof(NULL));
                (*args_list)[count][++count_commands] = NULL;
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

int check_background_work(char* string)
{
    int i = strlen(string) - 1;
    while(string[i] == ' ')
    {
        i--;
    }
    if(string[i] == '&')
    {
        return 1;
    }
    else
    {
        return 0;
    }
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

void close_pipes()
{
    for(int i=0;i<count_args-1;i++)
	{
		close(fd[i][0]);
		close(fd[i][1]);
	}
}

void free_arr_of_args(char**** args_list, int len)
{
    for(int i = 0; i != len; i++)
    {
        int j = 0;
        while((*args_list)[i][j] != NULL)
        {
            free((*args_list)[i][j++]);
        }
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
    char*** args; // [[[com, arg1, arg2 ...]], [[['&/n']], [com, arg1, arg2 ...]] ...]
    char* command_line;
    char* input_file_name;
    char* output_file_name;
    short output_file_flag;
    int input_file_flag;
    char out;
    short flag_nonstop = 1;
    while(flag_nonstop)
    {
        command_line = reade_string(command_line);
        if(strlen(command_line) == 0)
        {
            fprintf(stderr, "No commands\n");
            exit(1);
        }
        check_input_data(command_line);
        count_args = get_args_list(command_line, &args);
        input_file_flag = input_file_param(command_line, &input_file_name);
        output_file_flag = output_file_param(command_line, &output_file_name);
        if(count_args == 0)
        {
            fprintf(stderr, "No commands\n");
            exit(1);
        }
        malloc_pipes();
        if(check_background_work(command_line) == 0)
        {
            open_pipes();
            for(int i = 0; i != count_args; i++)
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
                            if(dup2(fd[i-1][0], 0) < 0)
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
                            if(dup2(fd[i][1], 1) < 0)
                            {
                                perror("dup2 output to pipe error");
                                exit(-1);
                            }
                        }
                    }
                    close_pipes();
                    execvp(args[i][0], args[i]);
                    fprintf(stderr, "failed to calling exec\n");
                    exit(1);
                }
            }
            wait(&unwwait);
        }
        else //background procces
        {
            open_pipes();
            for(int i = 0; i != count_args; i++)
            {
                int p = fork();
                if (p == -1)
                {
                    perror("fork error");
                    exit(-1);
                }
                if(p == 0)
                {
                    signal(SIGINT, SIG_IGN);
                    p = fork();
                    if (p == -1)
                    {
                        perror("fork error");
                        exit(-1);
                    }
                    if(p == 0)
                    {
                        signal(SIGINT, SIG_IGN);
                        if(input_file_flag && (i == 0)) //input block
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
                            if(i == 0)
                            {
                                if((input_bg_file = open("/dev/null", O_RDONLY, 0666)) < 0 )
                                {
                                    perror("input NULL oprn error");
                                    exit(-1);
                                }
                                if(dup2(input_bg_file, 0) < 0)
                                {
                                    perror("dup2 input from pipe error");
                                    exit(-1);
                                }
                            }
                            else
                            {
                                if(dup2(fd[i-1][0], 0) < 0)
                                {
                                    perror("dup2 input from pipe error");
                                    exit(-1);
                                }
                            }
                        }
                        if(output_file_flag && (i == count_args - 1)) //out block
                        {
                            if((output_file = open(output_file_name, O_WRONLY|O_CREAT)) < 0)
                            {
                                perror("cannot open output file");
                                exit(1);
                            }
                            if(dup2(output_file, 1) < 0)
                            {
                                perror("dup2 output to file error");
                                exit(-1);
                            }
                        }
                        else
                        {
                            if(i == count_args - 1)
                            {
                                if((out_bg_file = open("/dev/null", O_WRONLY, 0666)) < 0)
                                {
                                    perror("output NULL open error");
                                    exit(-1);
                                }
                                if(dup2(out_bg_file, 1) < 0)
                                {
                                    perror("dup2 output to pipe error");
                                    exit(-1);
                                }
                            }
                            else
                            {
                                if(dup2(fd[i][1], 1) < 0)
                                {
                                    perror("dup2 output from pipe error");
                                    exit(-1);
                                }
                            }
                        }
                        close_pipes();
                        execvp(args[i][0], args[i]);
                        fprintf(stderr, "failed to calling exec\n");
                        exit(1);
                    }
                    exit(0);
                }
                for(int i = 0; i != count_args; i++)
                {
                    wait(&unwwait);
                }
            }
        }
        close_pipes();
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
        sleep(1);
        printf("continue [Y/N]:\n");
        out = getchar();
        getchar();
        if(out == 'n' | out == 'N')
        {
            flag_nonstop = 0;
        }
        else
        {
            flag_nonstop = 1;
        }
    }
    exit(0);
}
>>>>>>> 1c09b09 (to mergen)
