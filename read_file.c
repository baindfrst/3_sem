#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void change_words(FILE* inp)
{
    int frst_pos = -1;
    int last_pos = -1;
    int readed;
    while((readed = getc(inp)) != EOF)
    {
        if(readed != ' ' && readed != '\n' && readed != '\t')
        {
            if(frst_pos == -1)
            {
                frst_pos = ftell(inp) - 1;
            }
            last_pos = ftell(inp) - 1;
        }
    }
    fseek(inp, last_pos, SEEK_SET);
    while((readed = getc(inp)) != ' ' && readed != '\n' && readed != '\t')
    {
        fseek(inp, -2, SEEK_CUR);
        if(ftell(inp) <= frst_pos)
        {
            break;
        }
    }
    last_pos = ftell(inp);
    if(last_pos <= frst_pos)
    {
        return;
    }
    char *frst;
    char *last;
    char *other;
    int i = -1;
    int j = -1;
    int k = -1;
    int middle_count = 0;
    fseek(inp, 0, SEEK_SET);
    while(ftell(inp) != frst_pos)
    {
        fread(&readed, sizeof(char), 1, inp);
        if (j == -1)
        {
            other = malloc(sizeof(char));
        }
        else
        {
            other = realloc(other, (j+2)*sizeof(char));
        }
        other[++j] = (char)readed;
    }
    fread(&readed, sizeof(char), 1, inp);
    while(readed != (int)' ' && readed != '\n' && readed != '\t')
    {
        if (i == -1)
        {
            frst = malloc(sizeof(char));
        }
        else
        {
            frst = realloc(frst, (i+2)*sizeof(char));
        }
        frst[++i] = (char)readed;
        fread(&readed, sizeof(char), 1, inp);
    }
    if (i == -1)
    {
        frst = malloc(sizeof(char));
    }
    else
    {
        frst = realloc(frst, (i+2)*sizeof(char));
    }
    frst[++i] = '\0';
    if (j == -1)
    {
        other = malloc(sizeof(char));
    }
    else
    {
        other = realloc(other, (j+2)*sizeof(char));
    }   
    other[++j] = (char)readed;
    middle_count++;
    while(ftell(inp) != last_pos)
    {
        fread(&readed, sizeof(char), 1, inp);
        if (j == -1)
        {
            other = malloc(sizeof(char));
        }
        else
        {
            other = realloc(other, (j+2)*sizeof(char));
        }
        other[++j] = (char)readed;
        middle_count++;
    }
    if (j == -1)
    {
        other = malloc(sizeof(char));
    }
    else
    {
        other = realloc(other, (j+2)*sizeof(char));
    }
    other[++j] = '\0';
    while((fread(&readed, sizeof(char), 1, inp) > 0) && readed != ' ' && readed != '\n' && readed != '\t')
    {
        if (k == -1)
        {
            last = malloc(sizeof(char));
        }
        else
        {
            last = realloc(last, (k+2)*sizeof(char));
        }
        last[++k] = (char)readed;
    }
    if (k == -1)
    {
        last = malloc(sizeof(char));
    }
    else
    {
        last = realloc(last, (k+2)*sizeof(char));
    }
    last[++k] = '\0';
    if (j == -1)
    {
        other = malloc(sizeof(char));
    }
    else
    {
        other = realloc(other, (j+2)*sizeof(char));
    }
    other[++j] = (char)readed;
    while((fread(&readed, sizeof(char), 1, inp)) > 0)
    {
        if (j == -1)
        {
            other = malloc(sizeof(char));
        }
        else
        {
            other = realloc(other, (j+2)*sizeof(char));
        }
        other[++j] = (char)readed;
    }
    fseek(inp, 0, SEEK_SET);
    int writed = 0;
    for(int ii = 0; ftell(inp) < frst_pos; ii++)
    {
        fwrite(&(other[ii]), sizeof(char), 1, inp);
        writed++;
    }
    for(int ii = 0; ii < strlen(last); ii++)
    {
        fwrite(&(last[ii]), sizeof(char), 1, inp);
    }
    for(int ii = 0; ii < middle_count ; ii++)
    {
        fwrite(&(other[ii + writed]), sizeof(char), 1, inp);
    }
    for(int ii = 0; ii < strlen(frst); ii++)
    {
        fwrite(&(frst[ii]), sizeof(char), 1, inp);
    }
    for(int ii = writed + middle_count; ii < strlen(other); ii++)
    {
        fwrite(&(other[ii]), sizeof(char), 1, inp);
    }
    free(frst);
    free(other);
    free(last);
}
  
int main()
{
    FILE* inp;
    inp = fopen("input.txt", "r+");
    change_words(inp);
    return 0;
}