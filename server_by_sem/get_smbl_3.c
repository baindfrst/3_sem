#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#define MSGSZ 128
struct sembuf sa;
struct sembuf sd;
char* open_shared_memory(char* name, char simb)
{
    key_t key_mem = ftok(name, simb);
    int shm = shmget(key_mem, MSGSZ, 0777);
    if (shm == -1)
    {
        perror("shm error\n");
        exit(1);
    }
    char *addr = shmat(shm, NULL, 0);
    if ( addr == (char*)-1 ) {
        perror("mmap error\n");
        exit(1);
    }
    return addr;
}

int main(int argc, char** argv)
{
    char* addr = open_shared_memory(argv[1], argv[2][0]);
    key_t k1 = ftok("f2.txt", 'b');
    int sem = semget(k1, 1, IPC_CREAT|0777);
    sa.sem_num = 0;
    sd.sem_num = 0;
    sa.sem_op = 2;
    sd.sem_op = -1;
    sa.sem_flg = 0;
    sa.sem_flg = 0;
    while(1)
    {
        if(shmget(ftok(argv[1], argv[2][0]), MSGSZ, 0777) < 1)
        {
            shmctl(ftok(argv[1], argv[2][0]), IPC_RMID, NULL);
            exit(1);
        }
        semop(sem, &sd, 1);
        char* string = addr;
        int count = 0;
        for(int i = 0; addr[i] != '\0'; i++)
        {
            if(addr[i] == ' ')
            {
                count++;
            }
        }
        char str[MSGSZ];
        if (count == 0)
        {
            str[0] = '0';
        }
        else
        {
            int i = 0;
            int buf = count;
            int len = 0;
            while(buf != 0)
            {
                buf /= 10;
                len++;
            }
            str[len + 1] = '\0';
            while(count != 0)
            {
                str[len - 1] = count % 10 + '0';
                count /= 10;
            }
        }
        memcpy(addr, str, strlen(str) + 1);
        semop(sem, &sa, 1);
    }

    return 0;
}
