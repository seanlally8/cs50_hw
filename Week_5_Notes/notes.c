#include <stdio.h>

    typedef struct node
    {
        int number;
        struct node *next;
    }
    node;
    
int main(void)
{
    node *list = NULL; 
    
    node *n = malloc(sizeof(node));
    if (n = NULL)
    {
        return 1;
    }
    n->number = 1;
    n->next = NULL;
    list = n;
    
    n = malloc(sizeof(node));
    if (n == NUll)
    {
        free(list);
        return 1;
    }
    n->number = 2;
    n->next = NULL;
    list->next = n;
    
    n = malloc(sizeof(node));
    if (n == NULL)
    {
        free(list->next);
        free(list);
        return 1;
    }
    n->number = 3;
    n-> next = NULL;
    list->next->next = n;
    
    for (node *tmp = list; tmp != NULL; tmp = tmp->next)//give myself temp pointer to beginning of list, so long as temp is not NULL
    {
        printf("%i\n", tmp->number);
    }
    
    while (list != NULL)
    {
        node *tmp = list->next;
        free(list);
        list = tmp;
    }
}