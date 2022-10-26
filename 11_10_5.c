#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct tree{
    int key;
    struct tree* left;
    struct tree* right;
} tree;

void add_to_tree(tree* obj, int elem)
{
    if (elem > obj->key)
    {
        if (obj->right != NULL)
        {
            add_to_tree(obj->right, elem);
        }
        else
        {
            tree* create = malloc(sizeof(tree));
            create->key = elem;
            create->right = NULL;
            create->left = NULL;
            obj->right = create;
        }
    }
    else
    {
        if (obj->left != NULL)
        {
            add_to_tree(obj->left, elem);
        }
        else
        {
            tree* create = malloc(sizeof(tree));
            create->key = elem;
            create->left = NULL;
            create->right = NULL;
            obj->left= create;
        }
    }
}

void free_tree(tree* input)
{
    if(input->left != NULL)
    {
        free_tree(input->left);
    }
    if(input->right != NULL)
    {
        free_tree(input->right);
    }
    free(input);
}

void print_tree(tree* from)
{
    if(from->right != NULL && from->left != NULL)
    {
        print_tree(from->left);
        printf("%d ", from->key);
        print_tree(from->right);
    }
    else
    {
        if(from->left != NULL && from->right == NULL)
        {
            print_tree(from->left);
            printf("%d ", from->key);
        }
        else
        {
            if(from->left == NULL && from->right != NULL)
            {
                printf("%d ", from->key);
                print_tree(from->right);
            }
            else
            {
                printf("%d ", from->key);
            }
        }
    }
}

int main(int argc, char** argv)
{
    tree* out = malloc(sizeof(tree));
    int created = 0;
    for(int i = 1; i != argc; i++)
    {
        FILE* file_in;
        if ((file_in = fopen(argv[i], "r")) == NULL)
        {
            printf("no file");
            return 0;
        }
        int readed;
        while (fscanf(file_in, "%d", &readed) > 0)
        {
            if(created == 0)
            {
                out->key = readed;
                out->left = NULL;
                out->right = NULL;
                created++;                
            }
            else
            {
                add_to_tree(out, readed);
            }
        }
        fclose(file_in);
    }
    if(created != 0)
    {
        print_tree(out);
    }
    free_tree(out);
    printf("\n");
    return 0;
}