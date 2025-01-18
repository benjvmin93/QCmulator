#ifndef LIST_H
#define LIST_H

#include <stddef.h>

struct List
{
    void *data;
    size_t data_size;
    struct List *next;
};

struct List *init_list(size_t data_size, size_t initial_size);
void free_list(struct List *head);
size_t list_size(struct List *head);
struct List *list_append(struct List *head, void *data);
void list_set(struct List *head, size_t index, void *data);
struct List *list_get(struct List *head, size_t index);

#endif