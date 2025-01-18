#include "headers/statevec.h"
#include "headers/alloc.h"
#include "headers/gate.h"

#include <stdio.h> 
void printBinary(int num) {
    for (int i = sizeof(int) * 8 - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}

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

struct Statevec *evolve_single(struct Statevec *sv, struct Gate *gate, unsigned char target)
{
    if (gate == NULL || sv == NULL)
    {
        return NULL;
    }

    unsigned char nqubits = sv->nqubits;
    int size = 1 << nqubits;
    int mask = 1 << (nqubits - target - 1);

    struct Complex **new_data = xmalloc(size * sizeof(struct Complex*));
    for (int i = 0; i < (1 << nqubits); ++i)
    {
        int index_op_high = (i & mask) != 0 ? 2 : 0;
        
        int index_cell0 = i & ~mask;
        int index_cell1 = i | mask;

        struct Complex *contrib_1 = complex_mul(gate->data[index_op_high], sv->data[index_cell0]);
        struct Complex *contrib_2 = complex_mul(gate->data[index_op_high | 1], sv->data[index_cell1]);

        new_data[i] = complex_add(contrib_1, contrib_2);
        free_complex(contrib_1);
        free_complex(contrib_2);
    }

    struct Statevec *result_sv = xmalloc(sizeof(struct Statevec));
    result_sv->nqubits = sv->nqubits;
    result_sv->data = new_data;

    return result_sv;
}