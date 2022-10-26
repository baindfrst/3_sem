#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
int main(int argc, char** argv)
{
    if (argc<3) {
        return 0;
    } 
    int n, filesize; char* s, *p;
    int f = open (argv[1],O_RDWR,0);
    n = strlen(argv[2]);
    if (f==-1) {
        perror("error");
        return 0;}
    if ((filesize = lseek(f,0,SEEK_END))>=1024) return 0;
    lseek(f,0,SEEK_SET);
    s = malloc(n*sizeof(char));
    int count = 1;
    int k=0;
    while (read(f,s,n*sizeof(char))== n)
    {  printf("%s",s);
        if (s == argv[2]) {
        p = malloc (sizeof(char)*(filesize -k - count*n));
        read(f,p,(filesize - k - count*n)*sizeof(char));
        lseek(f,k , SEEK_SET);
        write(f,p,(filesize-k-count*n)*sizeof(char));
        ftruncate(f,filesize-count*n);
        count++;
        lseek(f,k,SEEK_SET);
        free(p);
        } 
    else {
        lseek(f,-n+1, SEEK_CUR);
        k++;
        }
    }
    free(s);
    close(f);
    return 0;

}