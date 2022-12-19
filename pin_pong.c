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
    if(argc != 3)
    {
        perror("input data error\n");
        exit(0);
    }
    int fd_f_s[2];
    int fd_s_ss[2];
    int fd_ss_f[2];
    pid_t son;
    pid_t sson;
    int to = atoi(argv[2]);
    int from = atoi(argv[1]);
    if(from > to)
    {
        return 0;
    }
    int now = from;
    int take = to;
    pipe(fd_s_ss);
    pipe(fd_f_s);
    pipe(fd_ss_f);
    write(fd_ss_f[1], &now, sizeof(int));
    write(fd_ss_f[1], &take, sizeof(int));
    son = fork();
    sson = fork();
    if(son == 0)
    {
        if(sson != 0)
        {
            exit(1);
        }
    }
    while(1)
    {
        if(son == 0)
        {
            close(fd_f_s[1]);
            close(fd_s_ss[0]);
            close(fd_ss_f[0]);
            close(fd_ss_f[1]);
            read(fd_f_s[0], &now, sizeof(int));
            read(fd_f_s[0], &take, sizeof(int));
            if(now == take)
            {
                printf("%d s: %d\n", getpid(), now);
                now++;
                write(fd_s_ss[1], &now, sizeof(int));
                write(fd_s_ss[1], &take, sizeof(int));
                exit(1);
            }
            else
            {
                if(now == take + 1)
                {
                    write(fd_s_ss[1], &now, sizeof(int));
                    write(fd_s_ss[1], &take, sizeof(int));
                    exit(1);
                }
            }
            printf("%d s: %d\n", getpid(), now);
            now++;
            write(fd_s_ss[1], &now, sizeof(int));
            write(fd_s_ss[1], &take, sizeof(int));
        }
        else
        {
            if(sson == 0)
            {
                close(fd_s_ss[1]);
                close(fd_ss_f[0]);
                close(fd_f_s[0]);
                close(fd_f_s[1]);
                read(fd_s_ss[0], &now, sizeof(int));
                read(fd_s_ss[0], &take, sizeof(int));
                if(now == take)
                {
                    printf("%d ss: %d\n", getpid(), now);
                    now++;
                    write(fd_ss_f[1], &now, sizeof(int));
                    write(fd_ss_f[1], &take, sizeof(int));
                    exit(1);
                }
                else
                {
                    if (now == take + 1)
                    {
                        write(fd_ss_f[1], &now, sizeof(int));
                        write(fd_ss_f[1], &take, sizeof(int));
                        exit(1);
                    }
                }
                printf("%d ss: %d\n", getpid(), now);
                now++;
                write(fd_ss_f[1], &now, sizeof(int));
                write(fd_ss_f[1], &take, sizeof(int));
            }
            else
            {
                close(fd_ss_f[1]);
                close(fd_f_s[0]);
                close(fd_s_ss[0]);
                close(fd_s_ss[1]);
                read(fd_ss_f[0], &now, sizeof(int));
                read(fd_ss_f[0], &take, sizeof(int));
                if(now == take)
                {
                    printf("%d f: %d\n", getpid(), now);
                    now++;
                    write(fd_f_s[1], &now, sizeof(int));
                    write(fd_f_s[1], &take, sizeof(int));
                    exit(1);
                }
                else
                {
                    if(now == take + 1)
                    {
                        write(fd_f_s[1], &now, sizeof(int));
                        write(fd_f_s[1], &take, sizeof(int));
                        exit(1);
                    }
                }
                printf("%d f: %d\n", getpid(), now);
                now++;
                write(fd_f_s[1], &now, sizeof(int));
                write(fd_f_s[1], &take, sizeof(int));
            }
        }
    }
    return 1;
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
    if(argc != 3)
    {
        perror("input data error\n");
        exit(0);
    }
    int fd_f_s[2];
    int fd_s_ss[2];
    int fd_ss_f[2];
    pid_t son;
    pid_t sson;
    int to = atoi(argv[2]);
    int from = atoi(argv[1]);
    if(from > to)
    {
        return 0;
    }
    int now = from;
    int take = to;
    pipe(fd_s_ss);
    pipe(fd_f_s);
    pipe(fd_ss_f);
    write(fd_ss_f[1], &now, sizeof(int));
    write(fd_ss_f[1], &take, sizeof(int));
    son = fork();
    sson = fork();
    if(son == 0)
    {
        if(sson != 0)
        {
            exit(1);
        }
    }
    while(1)
    {
        if(son == 0)
        {
            close(fd_f_s[1]);
            close(fd_s_ss[0]);
            close(fd_ss_f[0]);
            close(fd_ss_f[1]);
            read(fd_f_s[0], &now, sizeof(int));
            read(fd_f_s[0], &take, sizeof(int));
            if(now == take)
            {
                printf("%d s: %d\n", getpid(), now);
                now++;
                write(fd_s_ss[1], &now, sizeof(int));
                write(fd_s_ss[1], &take, sizeof(int));
                exit(1);
            }
            else
            {
                if(now == take + 1)
                {
                    write(fd_s_ss[1], &now, sizeof(int));
                    write(fd_s_ss[1], &take, sizeof(int));
                    exit(1);
                }
            }
            printf("%d s: %d\n", getpid(), now);
            now++;
            write(fd_s_ss[1], &now, sizeof(int));
            write(fd_s_ss[1], &take, sizeof(int));
        }
        else
        {
            if(sson == 0)
            {
                close(fd_s_ss[1]);
                close(fd_ss_f[0]);
                close(fd_f_s[0]);
                close(fd_f_s[1]);
                read(fd_s_ss[0], &now, sizeof(int));
                read(fd_s_ss[0], &take, sizeof(int));
                if(now == take)
                {
                    printf("%d ss: %d\n", getpid(), now);
                    now++;
                    write(fd_ss_f[1], &now, sizeof(int));
                    write(fd_ss_f[1], &take, sizeof(int));
                    exit(1);
                }
                else
                {
                    if (now == take + 1)
                    {
                        write(fd_ss_f[1], &now, sizeof(int));
                        write(fd_ss_f[1], &take, sizeof(int));
                        exit(1);
                    }
                }
                printf("%d ss: %d\n", getpid(), now);
                now++;
                write(fd_ss_f[1], &now, sizeof(int));
                write(fd_ss_f[1], &take, sizeof(int));
            }
            else
            {
                close(fd_ss_f[1]);
                close(fd_f_s[0]);
                close(fd_s_ss[0]);
                close(fd_s_ss[1]);
                read(fd_ss_f[0], &now, sizeof(int));
                read(fd_ss_f[0], &take, sizeof(int));
                if(now == take)
                {
                    printf("%d f: %d\n", getpid(), now);
                    now++;
                    write(fd_f_s[1], &now, sizeof(int));
                    write(fd_f_s[1], &take, sizeof(int));
                    exit(1);
                }
                else
                {
                    if(now == take + 1)
                    {
                        write(fd_f_s[1], &now, sizeof(int));
                        write(fd_f_s[1], &take, sizeof(int));
                        exit(1);
                    }
                }
                printf("%d f: %d\n", getpid(), now);
                now++;
                write(fd_f_s[1], &now, sizeof(int));
                write(fd_f_s[1], &take, sizeof(int));
            }
        }
    }
    return 1;
>>>>>>> 1c09b09 (to mergen)
}