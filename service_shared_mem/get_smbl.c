#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#define MSGSZ 128


typedef struct msgbuf {
         long mtype;
         char mtext[MSGSZ];
         } message_buf;
key_t key2 = -1;
int que_simb = -1;
message_buf msgg;
message_buf prnt_count;

void IPC_for_put_id(char* str, char simb)
{
    key2 = ftok(str, simb);
    if (key2 == -1)
    {
        perror("key error");
        exit(1);
    }
    que_simb = msgget(key2, IPC_CREAT | 0666);
    if (que_simb == -1)
    {
        perror("que error");
        exit(2);
    }
}

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
    IPC_for_put_id(argv[1], argv[2][0]);
    while(1)
    {
        msgg.mtype = 1;
        if(msgrcv(que_simb, &msgg, sizeof(message_buf) - sizeof(long), 1, 0) < 0)
        {
            exit(1);
        }
        char* addr = open_shared_memory(argv[3]);
        char* string = addr;
        int count = 0;
        for(int i = 0; addr[i] != '\0'; i++)
        {
            if(addr[i] == ' ')
            {
                count++;
            }
        }
        prnt_count.mtype = 2;
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
        strcpy(prnt_count.mtext, "1");
        msgsnd(que_simb, &prnt_count, sizeof(message_buf) - sizeof(long), IPC_NOWAIT);
    }

    return 0;
}
