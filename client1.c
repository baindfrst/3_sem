/* CLIENT1 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#define N 4

typedef struct
 {
     long mestype;
     int mes[1];
 } messageto;
typedef struct
 {
     long mestype;
     char mes[256];
 } messagefrom;

messageto mesto;
messagefrom mesfrom;

int main(int argc, char **argv)
{
     key_t key;
     int mesid, count = 0, i;
     char S[256]; char* c = argv[2];
     key = ftok(argv[1], c[0]);
     if (key == -1)
     {
         perror ("ftok");
         exit(1);
     }
     mesid = msgget (key, IPC_CREAT |  0666);//IPC_CREAT | IPC_EXCL);
     mesfrom.mestype = 1;
     mesto.mestype = 2;
     //msgrcv(mesid, &mesfrom, 256, 1, 0);
     while (1)
     {
        if(msgrcv(mesid, &mesfrom, 256, 1, 0) < 0)
        {
            exit(0);
        }
         printf ("%s\n", mesfrom.mes);
         strcpy(S, mesfrom.mes);
         count = 0;
         for (i = 0; i< strlen(S); i++)
         {
             if ((S[i] <= '9') && (S[i] >= '0')) count++;
             //printf ("%c\n", S[i]);
         }
         printf("Count %d\n", count);
         mesto.mes[0] = count;
         msgsnd(mesid, &mesto, 2, 0);
     }

     return 0;
}
