#include "stdio.h"
#include <string.h>

int main()
{
    FILE* inp;
    inp = fopen("input.txt", "r");
    int readed;
    int status;
    status = fscanf(inp, "%d", &readed);
    printf("%d %d", readed, status);
    return 0;
}