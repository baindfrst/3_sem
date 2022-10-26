#include "stdio.h"
#include "limits.h"

void print_mas(int mas[], int len_mas)
{
    for( int i = 0; i != len_mas; i++)
    {
        printf("%d ", mas[i]);
    }
    printf("\n");
}

int *sum_mas(int* mas_1, int* mas_2, int len_1, int len_2)
{
    int* mas_out;
    char max_len = (len_1 > len_2);
    mas_out = malloc(sizeof(int)*(max_len?len_1:len_2));
    for(int i = 0; i != (max_len?len_2:len_1); i++)
    {
        mas_out[i] = mas_1[i] + mas_2[i];
        if (mas_out[i] < mas_2[i] && mas_1[i] > 0 && mas_2[i] > 0)
        {
            mas_out[i] = INT_MAX;
        }
        else
        {
            if (mas_out[i] > mas_1[i] && mas_1[i] < 0 && mas_2[i] < 0) 
            {
                mas_out[i] = INT_MIN;
            }
        }
    }
    if(max_len)
    {
        for (int i = len_2; i != len_1; i++)
        {
            mas_out[i] = mas_1[i];
        }
    }
    else
    {
        for (int i = len_1; i != len_2; i++)
        {
            mas_out[i] = mas_2[i];
        } 
    }
    return mas_out;
}

int main()
{
    int len_1, len_2;
    scanf("%d%d", &len_1, &len_2);
    int mas_1[len_1], mas_2[len_2];
    for (int i = 0; i != len_1; i ++)
    {
        scanf("%d", &mas_1[i]);
    }
    for (int i = 0; i != len_2; i ++)
    {
        scanf("%d", &mas_2[i]);
    }
    print_mas(mas_1, len_1);
    print_mas(mas_2, len_2);
    int* mas_fin;
    mas_fin = sum_mas(mas_1, mas_2, len_1, len_2);
    print_mas(mas_fin, (len_1>len_2?len_1:len_2));
    return 0;

}