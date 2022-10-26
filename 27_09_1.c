#include <stdio.h>
#include <stdlib.h>

typedef struct string{
    char elem;
    struct string* next;
    struct string* prev;
} string;

typedef struct list{
    string*  str;
    struct list* next;
    struct list* prev;
} list;

int count_char(string* inp)
{
    int out = 0;
    while (inp)
    {
        out++;
        inp = inp->next;
    }
    return out;
}

void free_str(string* inp)
{
    if (!inp)
    {
        return;
    }
    free_str(inp->next);
    free(inp);
}

void free_list(list* inp)
{
    if(!inp)
    {
        return;
    }
    free_list(inp->next);
    free_str(inp->str);
    free(inp);
}

int main()
{
    list* strings_list = malloc(sizeof(list));
    strings_list->next = NULL;
    strings_list->prev = NULL;
    strings_list->str = NULL;
    int readed;
    int flag = 0;
    readed = getchar();
    while ((readed != '\n') && readed != EOF)
    {
        list* string_add;
        if(strings_list->str != NULL)
        {
            string_add = malloc(sizeof(list));
            string_add->next = NULL;
            string_add->prev = strings_list;
            strings_list->next = string_add;
        }
        string* add = malloc(sizeof(string));
        add->next = NULL;
        add->prev = NULL;
        while(readed == ' ')
        {
            readed = getchar();
        }
        add->elem = (char)readed;
        readed = getchar();
        while ((readed != '\n') && readed != EOF && (char)readed != ' ')
        {
            string* p = malloc(sizeof(string));
            p->next = NULL;
            p->prev = add;
            p->elem = (char)readed;
            add->next = p;
            add = add->next;
            //free(p);
            readed = getchar();
        }
        while (add->prev != NULL)
        {
            add = add->prev;
        }
        if(strings_list->next == NULL && strings_list->prev == NULL)
        {
            strings_list->str = add;
        }
        else
        {
            string_add->str = add;
            strings_list = string_add;
            //free(string_list);
        }
    }
    while (strings_list->prev != NULL)
    {
        strings_list = strings_list->prev;
    }

    printf("1: ");
    int max = 0;
    while (strings_list->next != NULL)
    {
        int now = count_char(strings_list->str);
        printf("%d ", now);
        if (now > max)
        {
            max = now;
        }
        strings_list = strings_list ->next;
    }
    int now = count_char(strings_list->str);
    printf("%d ", now);
    printf("\n");
    if (now > max)
    {
        max = now;
    }
    
    while (strings_list->prev != NULL)
    {
        strings_list = strings_list->prev;
    }
    for(int i = 0; i != max; i++)
    {
        list* run;
        run = strings_list;
        while(run)
        {
            int flag_over = 0;
            for(int j = 0; j != i; j ++)
            {
                if(run->str->next != NULL)
                {
                    run->str = run->str->next;
                }
                else
                {
                    flag_over = 1;
                }
            }
            if (flag_over == 0)
            {
                printf("%c", run->str->elem);
            }
            else
            {
                printf("%c", ' ');
            }
            while(run->str->prev != NULL)
            {
                run->str = run->str->prev;
            }
            printf("%c", ' ');
            run = run->next;
        }
        printf("\n");
    }
    free_list(strings_list);
    return 0;
}