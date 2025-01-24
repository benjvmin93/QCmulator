#ifndef LIST_H
#define LIST_H

#include <stddef.h>

enum BUILTIN_TYPES
{
    CHAR=0,
    UCHAR,
    INT,
    SIZE_T,    
};

struct List
{
    void *data;
    size_t data_size;
    struct List *next;
};

struct List *init_list(size_t data_size, size_t initial_size);
struct List *init_list_from_array(void *array[], size_t initial_size, size_t data_size);
void free_list(struct List *head);
size_t list_size(struct List *head);    // List elements allocated.
size_t list_length(struct List *head);  // Number of data elements contained in the list.
struct List *list_append(struct List *head, void *data);
void list_set(struct List *head, size_t index, void *data);
struct List *list_get(struct List *head, size_t index);
void list_print(struct List *list, enum BUILTIN_TYPES data_type);

#endif