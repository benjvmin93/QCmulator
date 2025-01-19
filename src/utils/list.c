#include "../headers/list.h"
#include "../headers/alloc.h"

#include <stdio.h>

struct List *init_list(size_t data_size, size_t initial_size)
{
    if (initial_size == 0)
    {
        struct List *head = xmalloc(sizeof(struct List));
        head->data_size = data_size;
        head->data = NULL;
        head->next = NULL;
        return head;
    }

    struct List *head = xmalloc(sizeof(struct List));
    head->data_size = data_size;
    head->data = xmalloc(data_size);
    head->next = NULL;

    struct List *ptr = head;
    for (size_t i = 1; i < initial_size; ++i)
    {
        ptr->next = xmalloc(sizeof(struct List));
        ptr = ptr->next;
        ptr->data = xmalloc(data_size);
        ptr->data_size = data_size;
        ptr->next = NULL;
    }

    return head;
}

struct List *init_list_from_array(void *array[], size_t initial_size, size_t data_size)
{
    struct List *list = init_list(data_size, initial_size);
    for (size_t i = 0; i < initial_size; ++i)
    {
        list_set(list, i, array[i]);
    }

    return list;
}

void free_list(struct List *head)
{
    struct List *ptr = head;
    while (ptr)
    {
        if (ptr->data)
        {
            free(ptr->data);
        }

        struct List *tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
}

size_t list_size(struct List *head)
{
    size_t size = 0;
    struct List *ptr = head;
    while (ptr)
    {
        size++;
        ptr = ptr->next;
    }
    return size;
}

struct List *list_append(struct List *head, void *data)
{
    if (!head)
    {
        return NULL;
    }

    if (!head->data && head->next == NULL)
    {
        head->data = data;
        return head;
    }

    struct List *ptr = head;
    while (ptr->next)
    {
        ptr = ptr->next;
    }

    ptr->next = xmalloc(sizeof(struct List));
    ptr = ptr->next;
    ptr->data_size = head->data_size;
    ptr->data = data;
    ptr->next = NULL;

    return head;
}

void list_set(struct List *head, size_t index, void *data)
{
    struct List *list_element = list_get(head, index);
    if (list_element)
    {
        free(list_element->data);
        list_element->data = data;
    }
}

struct List *list_get(struct List *head, size_t index)
{
    struct List *ptr = head;

    for (size_t i = 0; i < index; ++i)
    {
        ptr = ptr->next;
    }

    return ptr;
}

void print_data(enum BUILTIN_TYPES data_type, void *data)
{
    if (!data)
    {
        printf("%p", data);
        return;
    }

    switch (data_type)
    {
        case CHAR:
            printf("%c", *(char *) data);
            break;
        case UCHAR:
            printf("%c", *(unsigned char *) data);
            break;
        case INT:
            printf("%d", *(int *) data);
            break;
        case SIZE_T:
            printf("%lu", *(size_t *) data);
            break;
        default:
            break;
    }
}

void list_print(struct List *list, enum BUILTIN_TYPES data_type)
{
    struct List *ptr = list;
    while (ptr)
    {
        print_data(data_type, ptr->data);
        printf(" -> ");
        ptr = ptr->next;
    }
    printf("\n");
}