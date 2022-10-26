#include "stdio.h"
#include <stdlib.h>

char* to_numb(char inp)
{
    char* out;
    out = malloc(sizeof(char));
    for (int i = 7; i != -1; i --)
    {
        if (inp % 2 == 1)
        {
            out[i] = '1';
        }
        else
        {
            out[i] = '0';
        }
        inp = inp / 2;
    }
    return out;
}

int count_task(char arr[], int len, int zero_len)
{
    int ret = 0;
    int count = 0;
    for (int i = 0; i != len; i++)
    {
        for (int j = 0; j != 8; j++)
        {
            int num_now = ((arr[i] << j) & 0x80) >> 7;
            printf("%d ", num_now);
            if (num_now == 0)
            {
                count++;
            }
            else
            {
                if (count >= zero_len)
                {
                    ret++;
                }
                count = 0;
            }
        }
        printf("\n");
    }
    return ret;
}
int ret_bit(char arr[], int len, int serch)
{
    return ((arr[serch/8] << (7 - (serch % 8))) & 0x80) >> 7;
}

int max_len(char arr[], int len)
{
    int ret = 0;
    int count = 0;
    for (int i = 0; i != len; i++)
    {
        for (int j = 0; j != 8; j++)
        {
            int num_now = ((arr[i] << j) & 0x80) >> 7;
            printf("%d ", num_now);
            if (num_now == 0)
            {
                count++;
            }
            else
            {
                if (count >= ret)
                {
                    ret = count;
                }
                count = 0;
            }
        }
        printf("\n");
    }
    return ret;
}

int main()
{
    char arr[100];
    int len;
    scanf("%d\n", &len);
    char* pr;
    for (int i = 0; i != len; i++)
    {
        scanf("%c", &arr[i]);
        for (int j = 0; j != 8; j++)
        {
            printf("%c", to_numb(arr[i])[j]);
        }
        printf(" ");
    }
    printf("\n");
    printf("%d\n", max_len(arr, len));
    printf("%d", ret_bit(arr, len, 13));
    return 0;
}