<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        perror("arg");
        return 1;
    }
    int file;
    int fd[2];
    int fd2[2];
    int c, c0, a, count =1, flag = 1;
    char *str, *str0;
    if ((file = open (argv[1], O_RDWR))<0)
    {
        perror("file");
        return 1;
    }
    int status = 0;
    pid_t  pid;
    pipe(fd);
    pipe(fd2);
    if ((pid =fork())== 0)
        {
            //���
            close(fd2[1]);
            dup2(fd2[0],0); //fd2 - �� ���� � ����,
                            //������ ���, ����� ������ �� ���� ����� �� ����� �����
            while (scanf("%c", &c)==0){}
            while (fgets(str, 80, stdin) == NULL){}
            while (*str)
            {
                if (c == *str) count++;
                str++;

            }
            dup2 (file, 1);
            printf ("%d", count); //���������� � ���� ���-�� �
            close(fd2[0]);
            close (fd[0]);
            dup2 (fd[1], 1);
            printf ("%d", 1); //������ ��������
            close (fd[0]);
            close (fd[1]);
            exit(0);
        }
    else
    {
        if (flag)
        {
            close (fd[0]);
            dup2(fd[1],1);
            printf ("%d", 1);
            close(fd[0]);
            close(fd[1]);
        }
        flag = 0;
        close(fd[1]);
        dup2(fd[0], 0); //����� ������ �� ����
        while (scanf("%d", &a)!=1){}
        while (a ==1)
        {
            close(fd[0]);
            close(fd[1]);
            while (!strcmp(str0, "exit"))
            {
                //close(fd2[1]);
                scanf("%c", c0);
                fgets (str0, 80, stdin);
                close(fd2[0]);
                dup2(file, 1);
                printf ("%s", str0);
                close (fd2[0]);
                dup2 (fd2[1], 1);
                printf ("%c", c0);
                printf("%s", str0);
                close(fd2[1]);
                close(fd2[0]);
                close(fd[1]);
                dup2(fd[0], 0); //����� ������ �� ����
                while (scanf("%d", &a)!=1){}
            }
            if (strcmp(str0, "exit")==0) break;

        }
        close(fd[1]);
        close(fd[0]);
        exit(0);

     }
     //������� ����
     close(file);
     return 0;
}
=======
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        perror("arg");
        return 1;
    }
    int file;
    int fd[2];
    int fd2[2];
    int c, c0, a, count =1, flag = 1;
    char *str, *str0;
    if ((file = open (argv[1], O_RDWR))<0)
    {
        perror("file");
        return 1;
    }
    int status = 0;
    pid_t  pid;
    pipe(fd);
    pipe(fd2);
    if ((pid =fork())== 0)
        {
            //���
            close(fd2[1]);
            dup2(fd2[0],0); //fd2 - �� ���� � ����,
                            //������ ���, ����� ������ �� ���� ����� �� ����� �����
            while (scanf("%c", &c)==0){}
            while (fgets(str, 80, stdin) == NULL){}
            while (*str)
            {
                if (c == *str) count++;
                str++;

            }
            dup2 (file, 1);
            printf ("%d", count); //���������� � ���� ���-�� �
            close(fd2[0]);
            close (fd[0]);
            dup2 (fd[1], 1);
            printf ("%d", 1); //������ ��������
            close (fd[0]);
            close (fd[1]);
            exit(0);
        }
    else
    {
        if (flag)
        {
            close (fd[0]);
            dup2(fd[1],1);
            printf ("%d", 1);
            close(fd[0]);
            close(fd[1]);
        }
        flag = 0;
        close(fd[1]);
        dup2(fd[0], 0); //����� ������ �� ����
        while (scanf("%d", &a)!=1){}
        while (a ==1)
        {
            close(fd[0]);
            close(fd[1]);
            while (!strcmp(str0, "exit"))
            {
                //close(fd2[1]);
                scanf("%c", c0);
                fgets (str0, 80, stdin);
                close(fd2[0]);
                dup2(file, 1);
                printf ("%s", str0);
                close (fd2[0]);
                dup2 (fd2[1], 1);
                printf ("%c", c0);
                printf("%s", str0);
                close(fd2[1]);
                close(fd2[0]);
                close(fd[1]);
                dup2(fd[0], 0); //����� ������ �� ����
                while (scanf("%d", &a)!=1){}
            }
            if (strcmp(str0, "exit")==0) break;

        }
        close(fd[1]);
        close(fd[0]);
        exit(0);

     }
     //������� ����
     close(file);
     return 0;
}
>>>>>>> 1c09b09 (to mergen)
