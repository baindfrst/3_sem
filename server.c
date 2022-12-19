/* SERVER */
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
     char mes[256];
 } messageto;
typedef struct
 {
     long mestype;
     int mes[1];
 } messagefrom;

messageto mesto;
messagefrom mesfrom;

char* readstr();
int main(int argc, char **argv)
{
     key_t key;
     char* S;
     int mesid;
     char* c = argv[2];
     key = ftok(argv[1], c[0]);
     if (key == -1)
     {
         perror ("ftok");
         exit(1);
     }
     mesid = msgget (key, IPC_CREAT |  0666);//| IPC_CREAT | IPC_EXCL);
     mesto.mestype = 1;
     mesfrom.mestype = 2;
     while(S = readstr())
     {

         strcpy(mesto.mes, S);
         msgsnd(mesid, &mesto, strlen(S)+1, 0);
         /*message.mestype = 2;
         msgsnd(mesid, &messageto, strlen(S)+1, 0);*/
         free (S);
         msgrcv(mesid, &mesfrom, sizeof(int), 2, 0);
         printf ("%d\n", mesfrom.mes[0]);

     }
    msgctl(mesid, IPC_RMID, NULL);
     return 0;

}
char* readstr(){
    char *S = malloc(N*sizeof(char));
    int i = 0;
    int j = 1;
    int c;

    while (((c=getchar()) != '\n') && (c != EOF))
    {
        S[i] = (char)c;
        i++;
        if (N * j == i)
        {
            S = realloc(S, ((sizeof(char))*(N + N*j)));
            j++;
        }
    }
    if (c == EOF) return NULL;
    S[i] = '\0';
    return S;

}
