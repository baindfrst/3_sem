#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#define MSGSZ 128

typedef struct msgbuf {
         long mtype;
         char mtext[MSGSZ];
         } message_buf;
key_t key1 = -1;
key_t key3 = -1;
int que_id = -1;
int que_simb = -1;
message_buf msgsend, scnd_send;
message_buf msgg;

void IPC_for_getting_id(char* str, char simb)
{
    key1 = ftok(str, simb);
    if (key1 == -1)
    {
        perror("key error");
        exit(1);
    }
    que_id = msgget(key1, IPC_CREAT | 0666);
    if (que_id == -1)
    {
        perror("que error");
        exit(2);
    }
}

void IPC_for_simb_id(char* str, char simb)
{
    key3 = ftok(str, simb);
    if (key3 == -1)
    {
        perror("key error");
        exit(1);
    }
    que_simb = msgget(key3, IPC_CREAT | 0666);
    if (que_simb == -1)
    {
        perror("que error");
        exit(2);
    }
}
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
    char* addr;
    IPC_for_getting_id(argv[2], argv[3][0]);
    IPC_for_simb_id(argv[4], argv[5][0]);
    addr = create_shared_memory(argv[6]);
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
        msgsend.mtype = 1;
        strcpy(msgsend.mtext, "1");
        memcpy(addr, string, sizeof(string) + 1);
        msgsnd(que_id, &msgsend, sizeof(message_buf) - sizeof(long), IPC_NOWAIT);
        msgg.mtype = 2;
        msgrcv(que_id, &msgg, sizeof(message_buf) - sizeof(long), 2, 0);
        printf("count of num %s\n", addr);
        scnd_send.mtype = 1;
        strcpy(scnd_send.mtext, "1");
        memcpy(addr, string, sizeof(string) + 1);
        msgsnd(que_simb, &scnd_send, sizeof(message_buf) - sizeof(long), IPC_NOWAIT);
        msgg.mtype = 2;
        msgrcv(que_simb, &msgg, sizeof(message_buf) - sizeof(long), 2, 0);
        printf("count of prob %s\n\n", addr);
    }
    msgctl(que_id, IPC_RMID, NULL);
    msgctl(que_simb, IPC_RMID, NULL);
    return 0;
}
