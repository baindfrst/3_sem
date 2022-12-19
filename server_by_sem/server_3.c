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
int shm;
char* create_shared_memory(char* name, char simb)
{
    key_t key_mem = ftok(name, simb);
    shm = shmget(key_mem, MSGSZ, IPC_CREAT|0777);
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
    int sem1_1, sem2_1, sem1_2, sem2_2;
    key_t k1, k2, k3, k4;
    k1 = ftok("f1.txt", 'a');//1i
    k2 = ftok("f2.txt", 'b');//2i
    sem1_1 = semget(k1, 1, IPC_CREAT|0777);
    sem2_1 = semget(k2, 1, IPC_CREAT|0777);
    semctl(sem1_1, 0, SETVAL, 0);
    semctl(sem2_1, 0, SETVAL, 0);
    sa.sem_num = 0;
    sd.sem_num = 0;
    sa.sem_op = 1;
    sd.sem_op = -2;
    sa.sem_flg = 0;
    sa.sem_flg = 0;
    char* addr;
    addr = create_shared_memory(argv[2], argv[3][0]);
    char string[MSGSZ + 1];
    FILE* file_inp;
    char* take_ans;
    if((file_inp = fopen(argv[1], "r")) == NULL)
    {
        perror("nofile");
        exit(1);
    }
    while(fgets(string, MSGSZ, file_inp))
    {
        printf("string: %s\n", string);
        memcpy(addr, string, sizeof(string) + 1);
        semop(sem1_1, &sa, 1);
        semop(sem1_1, &sd, 1);
        printf("count of num %s\n", addr);
        memcpy(addr, string, sizeof(string) + 1);
        semop(sem2_1, &sa, 1);
        semop(sem2_1, &sd, 1);
        printf("count of prob %s\n\n", addr);
    }
    shmctl(shm, IPC_RMID, NULL);
    semctl(sem1_1,1, IPC_RMID, NULL);
    semctl(sem2_1,1, IPC_RMID, NULL);
    return 0;
}
