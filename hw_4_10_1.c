#include <stdio.h>

int count_of_one(int inp)
{
    int count = 0;тттттбт
    for (int i = 0; i != 32; i++)
    {
        count += (((inp << i) & 0x80000000) >> 31);
    }
    return count;
}

void print_in_double(int inp)
{
    for (int i = 0; i != 32; i++)
    {
        printf("%d", (((inp << i) & 0x80000000) >> 31));
    }
    printf(" ");
}

void sort(int* arr, int len) // сортировка методом простого обмена
{
    for (int i = 0; i != len; i++)
    {
        int count_min = 32;
        int index_max = 0;
        int buf = 0;
        for (int j = i; j != len; j++)
        {
            if ((count_of_one(arr[j]) < count_min) || ((count_of_one(arr[j]) == count_min) && (arr[j] > arr[index_max])))
            {
                count_min = count_of_one(arr[j]);
                index_max = j;
            }
        }
        buf = arr[i];
        arr[i] = arr[index_max];
        arr[index_max] = buf;
    }
}

int main()
{
    int arr[100];
    int len;
    scanf("%d", &len);
    for (int i = 0; i != len; i++)
    {
        scanf("%d", &arr[i]);
    }
    for (int i = 0; i != len; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
        for (int i = 0; i != len; i++)
    {
        print_in_double(arr[i]);
    }
    printf("\n");
    sort(arr, len);
    for (int i = 0; i != len; i++)
    {
        printf("%d ", arr[i]);
    }
    return 0;
}