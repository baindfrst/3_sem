#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        exit(1);
    }
    FILE *input_file;
    if ((input_file = fopen(argv[1], "w")) == NULL)
    {
        exit(2);
    }
    int readed;
    while ((readed = getchar()) != EOF && readed != '\n' && readed != '\0')
    {
        char *string;
        int len = 0;
        while (readed != ' ' && readed != EOF && readed != '\n' && readed != '\0')
        {
            if (len == 0)
            {
                string = malloc(sizeof(char));
            }
            else
            {
                string = realloc(string, (len + 1) * sizeof(char));
            }
            string[len++] = (char)readed;
            readed = getchar();
        }
        if (len == 0)
        {
            string = malloc(sizeof(char));
        }
        else
        {
            string = realloc(string, (len + 1) * sizeof(char));
        }
        string[len++] = '\0';
        fputs(string, input_file);
        fputc(' ', input_file);
        free(string);
    }
    exit(0);
}