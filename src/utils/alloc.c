#include <err.h>
#include <errno.h>

#include "alloc.h"

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