#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void go_to_dir(DIR* input)
{
    struct dirent* check;
    DIR* dir_rec;
    char** dirs_arr;
    int size = 0;
    while(check = readdir(input))
    {
        if((dir_rec = opendir(check->d_name)) != NULL)
        {
            go_to_dir(dir_rec);
            closedir(dir_rec);
            // dirs_arr[size] = realloc(dirs_arr[size], sizeof(dirs_arr) + sizeof(check->d_name));
            // dirs_arr[size] = check->d_name;
            // closedir(dir_rec);
            // size++;
        }
        else
        {
            printf("%s\n", check->d_name);
        }
    }
    printf("\n");
    // for(int i = 0; i != size-1; i++)
    // {
    //     dir_rec = opendir(dirs_arr[i]);
    //     go_to_dir(dir_rec);
    //     closedir(dir_rec);
    // }
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
        printf("not cat");
    }

    struct dirent* rec;

    struct stat file_stat, file_stat_2;
    long long reg_size = 0;
    while(rec = readdir(dp))
    {
        lstat(rec->d_name, &file_stat);
        stat(rec->d_name, &file_stat_2);
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
            while(ch = readdir(back))
            {
                stat(ch->d_name, &file_stat_check);
                if(file_stat.st_ino == file_stat_check.st_ino)
                {
                    pass++;
                }
            }
            if(pass == 0)
            {
                printf("unknow-file: %s ", name);
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
    go_to_dir(dp);
    closedir(dp);
    return 0;
}