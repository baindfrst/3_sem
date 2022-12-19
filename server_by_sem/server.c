#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#define MSGSZ 128


char* create_shared_memory(char* name)
{
    int shm =  shm_open(name, O_CREAT|O_RDWR, 0777);
    if (shm == -1)
    {
        perror("shm error\n");
        exit(1);
    }
    if (ftruncate(shm, MSGSZ+1) == -1)
    {
        perror("mem allocate err\n");
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
    sem_t *sem1_1;
    sem_t *sem2_1;
    sem_t *sem1_2;
    sem_t *sem2_2;
    if ((sem1_1 = sem_open("/sem_frst_in", O_CREAT, 0777, 0)) == SEM_FAILED ) 
    {
        perror("sem 1 open error\n");
        exit(1);
    }
    if ((sem2_1 = sem_open("/semt_in", O_CREAT, 0777, 0)) == SEM_FAILED ) 
    {
        perror("sem 2 open error\n");
        exit(1);
    }
    if ((sem1_2 = sem_open("/sem_frst_out", O_CREAT, 0777, 0)) == SEM_FAILED ) 
    {
        perror("sem 1 open error\n");
        exit(1);
    }
    if ((sem2_2 = sem_open("/semt_out", O_CREAT, 0777, 0)) == SEM_FAILED ) 
    {
        perror("sem 2 open error\n");
        exit(1);
    }
    char* addr;
    addr = create_shared_memory(argv[2]);
    char string[MSGSZ + 1];
    FILE* file_inp;
    char* take_ans;
    if((file_inp = fopen(argv[1], "r")) == NULL)
    {
        perror("nofile");
        exit(1);
    }
    printf("%d", sem_wait(sem1_1));
    printf("%d", sem_wait(sem2_1));
    while(fgets(string, MSGSZ, file_inp))
    {   
        printf("string: %s\n", string);
        memcpy(addr, string, sizeof(string) + 1); 
        sem_post(sem1_2);
        sem_wait(sem1_1);
        printf("count of num %s\n", addr);
        memcpy(addr, string, sizeof(string) + 1);
        sem_post(sem2_2);
        sem_wait(sem2_1);
        printf("count of prob %s\n\n", addr);
    }
    sem_destroy(sem1_1);
    sem_destroy(sem2_1);
    sem_destroy(sem1_2);
    sem_destroy(sem2_2);
    return 0;
}
