//сука shell 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int** fd; 
int in_fd, out_fd;
int *Number_of_arguments;
int proc_num, back_num = 0; char* strcom;
char *in_file = NULL, *out_file = NULL;
char **simple_com, ***arguments;
int* arg_num;
int count_simbol (char *str,char c) ;
void ERR_op_file(int fd){
    if (fd==-1)
    {
        perror("Error open file");
        close(fd);
        exit(1);
    }
    return;
}
void input_com(){  
     unsigned part=4, size = 0, i=0;
    char c;

    strcom = (char*)malloc(size+=part);
    if (!strcom) {perror("alloc error"); exit(1);}
    while (((c = getchar())!=EOF)&&(c !='\n')&&(c!='\0'))
    {
        strcom[i++] = c;
        if (i==size) strcom = (char*)realloc(strcom,size+=part);
    }
    strcom[i]='\0';
    strcom = (char*)realloc(strcom,(i+1)*sizeof(c));
    
}
// void syntax_check () {}

void input_output ()
    {
	char *pointer;
	int j=1;
	if (count_simbol(strcom, '>'))
	{
		pointer=strrchr(strcom, '>');
		unsigned basis=4;
		unsigned size=0;
		unsigned i=0;
		char ch;
		out_file=(char*)malloc(size+=basis);
		if (!out_file)
		{
			perror("Error alloc memory \n");
			exit(1); 
		}
		while ((ch=pointer[j++])==' '){}
		while ((ch!=' ')&&(ch!=EOF)&&(ch!='\n')&&(ch!='\0')&&(ch!='<')&&(ch!='&'))
		{
			out_file[i++]=ch;
			if (i==size)
				out_file=(char*)realloc(out_file, size+=basis);
			ch=pointer[j++];
		}
		out_file[i]='\0';
		out_file=(char*)realloc(out_file, (i+1)*sizeof(ch));
		if (!strlen(out_file))
		{
			fprintf(stderr, "Error in the command syntax \n");
			exit(1);
		}
	}
	j=1;
	if (count_simbol(strcom, '<'))
	{
		pointer=strrchr(strcom, '<');
		unsigned basis=4;
		unsigned size=0;
		unsigned i=0;
		char ch;
		in_file=(char*)malloc(size+=basis);
		if (!in_file)
		{
			perror("Error alloc memory \n");
			exit(1); 
		}
		while ((ch=pointer[j++])==' '){}
		while ((ch!=' ')&&(ch!=EOF)&&(ch!='\n')&&(ch!='\0')&&(ch!='<')&&(ch!='&'))
		{
			in_file[i++]=ch;
			if (i==size)
				in_file=(char*)realloc(in_file, size+=basis);
			ch=pointer[j++];
		}
		in_file[i]='\0';
		in_file=(char*)realloc(in_file, (i+1)*sizeof(ch));
		if (!strlen(in_file))
		{
			fprintf(stderr, "Error in the command syntax \n");
			exit(1);
		}
	}
}

int count_simbol (char *str,char c) {
    int count = 0, i=0;
    while (i<strlen(str))
    {
        if (str[i++]==c) count+=1;
    }
    return count;
}
void pipes_mem()
{
    fd=(int **)malloc((proc_num-1)*sizeof(int *));
	if (!fd)
	{
		perror("Error alloc memory \n");
		exit(1); 
	}
	for(int i=0;i<proc_num-1;i++)
	{
		fd[i]=(int *)malloc(2*sizeof(int));
		if (!fd[i])
		{
			perror("Error alloc memory \n");
			exit(1); 
		}
	}
} 
void com_to_simple_com() //parsing
{  
    simple_com=(char **)malloc((proc_num)*sizeof(char *));
	if (!simple_com)
	{
		perror("Error alloc memory \n");
		exit(1); 
	}
	int i=0;
	char *str=strtok(strcom, "|");
	while (str!=NULL)
	{
		simple_com[i]=(char *)malloc((strlen(str)+1)*sizeof(char));
		if (!simple_com[i])
		{
			perror("Error alloc memory \n");
			exit(1); 
		}
		strcpy(simple_com[i], str);
		str=strtok(NULL, "|");
		i+=1;
	}	
   proc_num = count_simbol(strcom,'|');
}
int count_arg_sc(char*string)
{
    char *copy;
	int i=1;
	copy=(char *)malloc((strlen(string)+1)*sizeof(char));
	strcpy(copy, string);
	char *str=strtok(copy, " ");
	while (str!=NULL)
	{
		if ((count_simbol(str, '&'))||(count_simbol(str, '<'))||(count_simbol(str, '>')))
			break;
		str=strtok(NULL, " ");
		i++;;
	}	
	free(copy);
	return i;
}
void simple_com_to_arg()
{
	arg_num=(int *)malloc(proc_num*sizeof(int));
	arguments=(char ***)malloc((proc_num)*sizeof(char **));
    if (!arguments)
	{
		perror("Error alloc memory \n");
		exit(1); 
	}
	for(int i=0;i<proc_num;i++)
    {
		arg_num[i]=count_arg_sc(simple_com[i]);

		arguments[i]=(char **)malloc((arg_num[i])*sizeof(char *));
		if (!arguments[i])
		{
			perror("Error alloc memory \n");
			exit(1); 
		}

        int j = 0;
        char *str = strtok (simple_com[j], " ");
        while (str != NULL)
        {
            arguments[i][j] = (char *) malloc ((strlen(str)+1)*sizeof(char));
            if (!arguments[i][j])
            {
                perror("Error alloc memory \n");
                exit(1);
            }
            strcpy(arguments[i][j],str);
            str = strtok(NULL, " ");
            i++;
            if (i ==arg_num[i] - 1 ) break;
        }
        arguments[i][arg_num[i] - 1] = NULL;
    }
}
void Free_allocation ()
{
    /*if //flag in
        free (in_file);
    if //flag out
        free (out_file);*/
    if (strcom) 
        free(strcom);
    for (int i =0; i<proc_num-1;i++)
        free(fd[i]);
    free(fd);
    for (int i =0; i<proc_num;i++)
        free(simple_com[i]);
    free(simple_com);
    for (int i=0;i<proc_num;i++)
	{
		for(int j=0;j<arg_num[i]-1;j++)
			free(arguments[i][j]);
		free(arguments[i]);
	}


    
}
int main (int argc, char** argv){
pid_t pid;
int bg_in_fd, bg_out_fd; 
    while (1) {
        printf("Enter: ");
        input_com();
        //checking sintax
        input_output();
        pipes_mem();
        com_to_simple_com();
        simple_com_to_arg();

        if (!count_simbol(strcom,'&')) // if not background
        {  
            for (int i=0; i<proc_num -1;i++) // open pipes
            { int p;
                if ( (p = pipe(fd[i]))==-1) 
                {   
                    perror("Error calling pipe");
                    exit(1);        
                }
            }
            for (int i = 0;i<proc_num;i++) // begin proces
            { 
                if (( pid = fork())   ==-1)
                    {
                        fprintf(stderr, "fork error  \n");
                        kill(0, SIGKILL);
                        exit(1);				
                    }
                if (pid ==0) 
                {
                    if (i==0)
                    {
/*do files*/          if (count_simbol(strcom,'<'))// check  < vvod , перенаправление ввода
                        { 
                            in_fd=open(in_file, O_RDONLY, 0660);
                            ERR_op_file(in_fd);
                            dup2(in_fd, 0);
                            close(in_fd); 
                        }
                        if ((proc_num == 1) || (i == proc_num-1))
                        {
                            if (count_simbol(strcom,'>'))  // перенаправление вывода
                            { 
                                if (count_simbol(strcom,'>')==1) 
                                out_fd = open(out_file,O_WRONLY|O_CREAT|O_TRUNC, 0660);
                                else if (count_simbol(strcom,'>')==2) 
                                out_fd = open(out_file,O_APPEND|O_CREAT|O_WRONLY, 0660);
                                ERR_op_file(out_fd);
                                dup2(out_fd,1);
                                close(out_fd);
                            }
                            if (i == proc_num-1) //последний процесс
                                {
                                    dup2(fd[i-1][0], 0);
                                    for(int i=0;i<count_simbol-1;i++)
                                    {
                                        close(fd[i][0]);
                                        close(fd[i][1]);
                                    }	
                                }
                            execvp(arguments[i][0], arguments[i]);
                            perror("exec Error");
                            exit(1);
                        }
                        dup2(fd[i][1],1); //массив дескрипторов
                        for(int i=0;i<proc_num-1;i++)
                            {
                                close(fd[i][0]);
                                close(fd[i][1]);
                            }	
                        execvp(arguments[i][0], arguments[i]);
                        perror("Error exec");
                        exit(1);
                    }
                    else if ((i>0)&&(i<(proc_num-1)))
                    { 
                        dup2(fd[i-1][0],0);
                        dup2(fd[1][1],1);
                        for(int i=0;i<proc_num-1;i++)
                            {
                                close(fd[i][0]);
                                close(fd[i][1]);
                            }	
                        execvp(arguments[i][0], arguments[i]);
                        perror("exec Err");
                        exit(1);
                    } 
                }
            } 
            for(int i=0;i<proc_num-1;i++)
            {
                close(fd[i][0]);
                close(fd[i][1]);
            }
            for(int i=0;i<proc_num;i++)
            {
                wait(NULL);
            }	
            //прописать освобождение памяти
        } 
        else //background
        {  
            back_num+=1;
            if (( pid = fork())   ==-1)
                    {
                        fprintf(stderr, "fork error  \n");
                        kill(0, SIGKILL);
                        exit(1);				
                    }
            if (pid==0)
            { //son
                signal(SIGINT, SIG_IGN);
                if (( pid = fork())   ==-1)
                    {
                        fprintf(stderr, "fork error  \n");
                        kill(0, SIGKILL);
                        exit(1);				
                    }
                if (pid == 0)
                {//grandson
                    signal(SIGINT, SIG_IGN);
                    for (int i=0; i<proc_num -1;i++) // open pipes
                    {   
                        int p;
                        if ( (p = pipe(fd[i]))==-1) 
                        {   
                            perror("Error calling pipe");
                            exit(1);        
                        }
                    }
                    for (int i=0; i< proc_num; i++)
                    {
                       if (( pid = fork())   ==-1)
                        {
                            fprintf(stderr, "fork error  \n");
                            kill(0, SIGKILL);
                            exit(1);				
                        }
                        if (pid == 0)
                        {
                        if (i==0)
                        {
                            if (count_simbol(strcom,'<'))// check  < vvod , перенаправление ввода
                            { 
                                in_fd=open(in_file, O_RDONLY, 0660);
                                ERR_op_file(in_fd);
                                dup2(in_fd, 0);
                                close(in_fd); 
                            }
                            else
                            { 
                                bg_in_fd = open("dev/null", O_RDONLY,0666);
                                ERR_op_file(bg_in_fd); 
                                dup2(bg_in_fd,0);
                                close(bg_in_fd);
                            }
                            if ((proc_num == 1)|| (i == proc_num -1))
                        {
                            if (count_simbol(strcom,'>'))  // перенаправление вывода
                            { 
                                if (count_simbol(strcom,'>')==1) 
                                    out_fd = open(out_file,O_WRONLY|O_CREAT|O_TRUNC, 0660);
                                else if (count_simbol(strcom,'>')==2) 
                                    out_fd = open(out_file,O_APPEND|O_CREAT|O_WRONLY, 0660);
                                ERR_op_file(out_fd);
                                dup2(out_fd,1);
                                close(out_fd);
                            } 
                            else
                            {
                                bg_out_fd = open("dev/null", O_WRONLY,0666);
                                ERR_op_file(bg_out_fd);
                                dup2(bg_out_fd,1);
                                close(bg_out_fd);
                            } 
                            if (i == proc_num -1) 
                            {dup2(fd[i-1][0], 0);
								for(int i=0;i<proc_num-1;i++)
                                {
                                    close(fd[i][0]);
                                    close(fd[i][1]);
                                }}
                            execvp(arguments[i][0], arguments[i]);
                            perror("exec error\n");
                            exit(1);
                        }
                    dup2(fd[i][1],1);
                    for(int i=0;i<proc_num-1;i++)
                        {
                            close(fd[i][0]);
                            close(fd[i][1]);
                        }
                    execvp(arguments[i][0], arguments[i]);
                    perror("exec error\n");
                    exit(1);
                     }
                     else if ((i>0)&&(i<proc_num -1)) 
                     {
                        dup2(fd[i-1][0], 0);
						dup2(fd[i][1], 1);
                        for(int i=0;i<proc_num-1;i++)
                            {
                                close(fd[i][0]);
                                close(fd[i][1]);
                            }
						execvp(arguments[i][0], arguments[i]);
						perror("exec error\n");
						exit(1);
                     }
                
                    }
                }
                for(int i=0;i<proc_num-1;i++)
                {
                    close(fd[i][0]);
                    close(fd[i][1]);
                }
                for(int i=0;i<proc_num;i++)
                {
                    wait(NULL);
                }
                Free_allocation ();
                exit(0);

            }
            exit(0);
        } 
        wait(NULL); 
    }
    sleep(1);
    printf("\nPress space ");
    //if space - work else not work
}
exit (0);
}
