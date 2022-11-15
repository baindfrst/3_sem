#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

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

void go_to_dir(char* dir_path, int deap)
{
    struct dirent* check;
    DIR* dir_rec;
    DIR* input = opendir(dir_path);
    char** dirs_arr;
    int size = 0;
    check = readdir(input);
    check = readdir(input);
    while(check = readdir(input))
    {
        char* adding = malloc(sizeof(dir_path)+sizeof(check->d_name) + 1);
        strcpy(adding, dir_path);
        strcat(adding, "/");
        strcat(adding, check->d_name);
        if((dir_rec = opendir(adding)) != NULL)
        {
            struct stat *st = malloc(sizeof(struct stat));
            stat(adding, st);
            time_t opening = st->st_atime;
            printf("%*c", deap + 1, ' '); //надо еще вывести время создания
            printf("dir: %s %s", adding, ctime(&opening));
            go_to_dir(adding, deap + 4);
            closedir(dir_rec);
            free(st);
        }
        else
        {
            struct stat *st = malloc(sizeof(struct stat));
            stat(adding, st);
            if((*st).st_mode & S_IXUSR)
            {
                printf("%*c", deap + 1, ' ');
                printf("%s\n", check->d_name);
            }
            free(st);
        }
        free(adding);
    }
    closedir(input);
}

int main (int argc, char** argv)
{
    if(argc!=2)
    {
        printf("error");
        return 1;
    }
    DIR* dp;
    if ((dp = opendir(argv[1])) == NULL)
    {
        FILE* inp = fopen(argv[1], "r+");
        change_words(inp);
        fclose(inp);
        return 0;
    }

    struct dirent* rec;

    struct stat file_stat, file_stat_2;
    long long reg_size = 0;
    rec = readdir(dp);
    rec = readdir(dp);
    while(rec = readdir(dp))
    {
        char *fullname = malloc(sizeof(argv[1]) + 1 + sizeof(rec->d_name));
        strcpy(fullname, argv[1]);
        strcat(fullname, "/");
        strcat(fullname, rec->d_name);
        lstat(fullname, &file_stat);
        stat(fullname, &file_stat_2);
        free(fullname);
        char* name = rec->d_name;
        if(S_ISLNK(file_stat.st_mode))
        {
            char name[260];
            for (int i = 0; rec->d_name[i] != '\0'; i++)
            {
                name[i] = rec->d_name[i];
                if(rec->d_name[i+1] == '\0')
                {
                    name[i+1] = '\0';
                }
            }
            DIR* back = dp;
            struct stat file_stat_check;
            rewinddir(back);
            struct dirent* ch;
            int pass = 0;
            ch = readdir(back);
            ch = readdir(back);
            while(ch = readdir(back))
            {
                char *name_ch;
                char *fullname_file = malloc(sizeof(argv[1]) + 1 + sizeof(ch->d_name));
                strcpy(fullname_file, argv[1]);
                strcat(fullname_file, "/");
                strcat(fullname_file, ch->d_name);
                name_ch = malloc(sizeof(ch->d_name));
                strcpy(name_ch, ch->d_name);
                stat(fullname_file, &file_stat_check);
                if(file_stat_2.st_ino == file_stat_check.st_ino && strcmp(name, name_ch))
                {
                    pass++;
                    printf("link: %s, go to: %s\n", name, name_ch);
                    free(fullname_file);
                    free(name_ch);
                    break;
                }
                free(fullname_file);
                free(name_ch);
            }
            if(pass == 0)
            {
                printf("unknow-file: %s \n", name);
            }
            rewinddir(back);
            ch = readdir(back);
            while (ch->d_name != rec->d_name)
            {
                ch = readdir(back);
            }
        }
        else
        {
            reg_size += file_stat.st_size;
        }
    }
    printf("\n size of all files: %llu", reg_size);
    closedir(dp);
    struct stat *st = malloc(sizeof(struct stat));
    stat(argv[1], st);
    time_t opening = st->st_atime;
    printf("%s", argv[1], ctime(&opening));
    free(st);
    go_to_dir(argv[1], 1);
    return 0;
}