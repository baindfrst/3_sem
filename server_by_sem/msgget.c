#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#define MSGSZ 128

char* open_shared_memory(char* str)
{
    int shm =  shm_open(str, O_RDWR, 0777);
    if (shm == -1)
    {
        perror("shm error\n");
        exit(1);
    }
    char* addr = mmap(0, MSGSZ+1, PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);
    if ( addr == (char*)-1 ) {
        perror("mmap error\n");
        exit(1);
    }
    return addr;
}

int main(int argc, char** argv)
{
    printf("start\n");
    char* addr = open_shared_memory(argv[1]);
    printf("addr\n");
    sem_t *sem;
    sem_t *sem2;
    printf("gg\n");
    if ((sem = sem_open("/sem_frst_in", 0)) == SEM_FAILED ) 
    {
        perror("sem_open");
        exit(1);
    }
    if ((sem2 = sem_open("/sem_frst_out", 0)) == SEM_FAILED ) 
    {
        perror("sem_open");
        exit(1);
    }
    printf("nice\n");
    sem_post(sem);
    while(1)
    {
        sem_wait(sem2);
        char* string = addr;
        int count = 0;
        for(int i = 0; addr[i] != '\0'; i++)
        {
            if(addr[i] >= '0' && addr[i] <= '9')
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
        sem_post(sem);
    }
    return 0;
}
