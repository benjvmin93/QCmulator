#include "headers/statevec.h"
#include "headers/alloc.h"

struct Statevec *init_statevec(unsigned char nqubits)
{
    if (nqubits == 0)
    {
        return NULL;
    }

    struct Statevec *sv = xmalloc(sizeof(struct Statevec));
    sv->nqubits = nqubits;
    size_t size = 1 << nqubits;
    sv->data = xmalloc(size * sizeof(struct Complex*));
    
    sv->data[0] = init_complex(1., 0.);
    for (size_t i = 1; i < size; ++i)
    {
        sv->data[i] = init_complex(0., 0.);
    }

    return sv;
}

void free_statevec(struct Statevec *sv)
{
    unsigned char nqubits = sv->nqubits;
    size_t size = 1 << nqubits;
    for (size_t i = 0; i < size; ++i)
    {
        free_complex(sv->data[i]);
    }
    free(sv->data);
    free(sv);
}