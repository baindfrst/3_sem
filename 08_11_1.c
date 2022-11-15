#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        exit(1);
    }
    FILE* input_file;
    if((input_file = fopen(argv[1], "r")) == NULL)
    {
        exit(2);
    }
    int readed = getc(input_file);
    int i = 1;
    while(readed != EOF)
    {
        char* string;
        int len = 0;
        while(readed != '\n' && readed != EOF)
        {
            if(len == 0)
            {
                string = malloc(sizeof(char));
            }
            else
            {
                string = realloc(string, (len + 1)*sizeof(char));
            }
            string[len++] = (char)readed;
            readed = getc(input_file);
        }
        if(len == 0)
        {
            string = malloc(sizeof(char));
        }
        else
        {
            string = realloc(string, (len + 1)*sizeof(char));
        }
        string[len++] = '\0';
        if(i % 2 == 0)
        {
            printf("%s\n", string);
            i++;
        }
        else
        {
            i++;
        }
        free(string);
        if(readed != EOF)
        {
            readed = getc(input_file);
        }
    }
    fclose(input_file);
    exit(0);
}