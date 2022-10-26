#include <stdio.h>
#include <stdlib.h>

typedef struct str_starts
{
    int start;
    int len;
    struct str_starts* prev;
    struct str_starts* next;
} str_starts;

int main(int argc, char** argv)
{
    FILE *file_in;
    char* name = argv[1];
    if ((file_in = fopen(name, "r+")) == NULL)
    {
        printf("no file");
        return 0;
    }
    char readed[100];
    int count = 0;
    int inp;
    str_starts* arr = malloc(sizeof(str_starts));
    arr->start = 0;
    arr->len = 0;
    arr->prev = NULL;
    while((inp = fgetc(file_in)) != EOF)
    {
        arr->len++;
        if(inp == '\n')
        {
            arr->len--;
            count++;
            str_starts* added = malloc(sizeof(str_starts));
            added->prev = arr;
            added->len = 0;
            arr->next = added;
            added->start = ftell(file_in);
            arr = arr->next;
        }
    }
    arr->next = NULL;
    fseek(file_in, -1, SEEK_CUR);
    if (inp = fgetc(file_in) != '\n')
    {
        count++;
        fseek(file_in, -1, SEEK_CUR);
        fputc('\n', file_in);
    }
    printf("%d\n", count);
    printf("string nb: ");
    while (arr->prev != NULL)
    {
        arr = arr->prev;
    }
    int str_num_print;
    scanf("%d", &str_num_print);
    if (str_num_print > count)
    {
        printf("ERROR\n");
    }
    else
    {
        for(int i = 0; i != str_num_print - 1; i ++)
        {
            arr = arr->next;
        }
        fseek(file_in, arr->start, SEEK_SET);
        while ((inp = fgetc(file_in)) != '\n')
        {
            printf("%c", (char)inp);
        } 
    }
    while(arr->prev != NULL)
    {
        arr = arr->prev;
    }
    FILE* file_out;
    file_out = fopen("out.txt", "w+");
    for (int i = 0; i != count; i++)
    {
        str_starts* cur = arr;
        cur = cur->next;
        str_starts* change = arr;
        while(cur->next != NULL)
        {
            if (arr->len > cur->len)
            {
                change = cur;
            }
            cur = cur->next;
        }

        cur = change->next;
        str_starts* pr = change->prev;
        str_starts* arr_next = arr->next;
        str_starts* arr_prev = arr->prev;
        if (arr->next != change)
        {
            change->next = arr->next;
            change->prev = arr->prev;
            if(change->prev)
            {
                arr_prev->next = change;
            }
            arr_next->prev = change;
            arr->prev = pr;
            arr->next = cur;
            if(arr->prev)
            {
                arr->prev->next = arr;
            }
            arr->next->prev = arr;
        }
        else
        {
            arr->next = change->next;
            change->prev = arr->prev;
            change->next = arr;
            arr->prev = change;
            if(arr->next)
            {
                arr->next->prev = arr;
            }
            if(change->prev)
            {
                change->prev->next = change;
            }
        }
        while(arr->prev != NULL)
        {
            arr = arr->prev;
        }
        for(int j = 0; j != i+1; j++)
        {
            arr = arr->next;
        }
    }
    while(arr->prev != NULL)
    {
        arr = arr->prev;
    }
    while(arr->next != NULL)
    {
        fseek(file_in, arr->start, SEEK_SET);
        while((inp = fgetc(file_in)) != '\n')
        {
            fputc((int)inp, file_out);
        }
        fputc((int)inp, file_out);
        arr = arr->next;
    }
    
    return 0;
}