#include <err.h>
#include <errno.h>

#include "../headers/alloc.h"

void *xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
    {
        err(errno, "xmalloc");
    }
    return ptr;

}
void *xcalloc(size_t nmemb, size_t size)
{
    void *ptr = calloc(nmemb, size);
    if (!ptr)
    {
        err(errno, "xmalloc");
    }
    return ptr;
}

int *int_alloc(int a)
{
    int *x = xmalloc(sizeof(int));
    *x = a;
    return x;
}

unsigned char *uchar_alloc(unsigned char a)
{
    unsigned char *x = xmalloc(sizeof(unsigned char));
    *x = a;
    return x;
}