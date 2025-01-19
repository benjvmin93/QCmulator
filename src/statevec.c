#include "headers/statevec.h"
#include "headers/alloc.h"
#include "headers/gate.h"
#include "headers/projector.h"

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
    sv->measurements = NULL;
    size_t size = 1 << nqubits;
    
    sv->data = xmalloc(size * sizeof(double complex));
    sv->data[0] = 1;
    for (size_t i = 1; i < size; ++i)
    {
        sv->data[i] = 0;
    }

    return sv;
}

void free_statevec(struct Statevec *sv)
{
    if (sv->measurements)
    {
        free_list(sv->measurements);
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

    double complex *new_data = xmalloc(size * sizeof(double complex));
    for (int i = 0; i < (1 << nqubits); ++i)
    {
        int index_op_high = (i & mask) != 0 ? 2 : 0;
        
        int index_cell0 = i & ~mask;
        int index_cell1 = i | mask;

        new_data[i] = gate->data[index_op_high] * sv->data[index_cell0] + gate->data[index_op_high | 1] * sv->data[index_cell1];
    }

    struct Statevec *result_sv = xmalloc(sizeof(struct Statevec));
    result_sv->nqubits = sv->nqubits;
    result_sv->data = new_data;
    result_sv->measurements = sv->measurements;

    return result_sv;
}

struct Statevec *evolve(struct Statevec *sv, struct Gate *gate, struct List *targets)
{
    if (gate == NULL || sv == NULL || targets == NULL)
    {
        return NULL;
    }

    unsigned char nqubits_sv = sv->nqubits;
    unsigned char nqubits_gate = gate->nqubits;
    size_t nTargets = list_size(targets);

    if (nqubits_gate != nTargets)
    {
        fprintf(stderr, "The number of targets is not consistant with the gate.\n");
        print_gate(gate);
        return NULL;
    }

    int mask = 0;

    struct List *tmp_targets = targets;
    while (tmp_targets)
    {
        int *target = tmp_targets->data;
        mask |= (1 << (int) nqubits_sv - *target - 1);
        tmp_targets = tmp_targets->next;
    }

    int size = 1 << nqubits_sv;
    double complex *new_data = xmalloc(size * sizeof(double complex));
    for (int i = 0; i < size; ++i)
    {
        int i_base = i & ~mask;
        double complex sum = 0;
        for (int j = 0; j < (1 << nqubits_gate); ++j)
        {
            int maskJ = 0;
            for (int t = 0; t < nTargets; ++t)
            {
                int bit = (j >> (nTargets - t - 1)) & 1;
                struct List *list_element = list_get(targets, t);
                int *target = list_element->data;
                int bitmask = bit << (nqubits_sv - *target - 1);
                maskJ |= bitmask;
            }
            int index = i_base | maskJ;
            int rowIndex = 0;
            for (int t = 0; t < nTargets; ++t)
            {
                struct List *list_element = list_get(targets, t);
                int *target = list_element->data;
                int bit = (i >> (nqubits_sv - *target - 1)) & 1;
                bit = bit << nTargets - t - 1;
                rowIndex |= bit;
            }

            int colIndex = j;

            int data_index = rowIndex * (1 << nqubits_gate) + colIndex; // Debug
            sum += gate->data[data_index] * sv->data[index];
        }

        new_data[i] = sum;
    }

    struct Statevec *result_sv = xmalloc(sizeof(struct Statevec));
    result_sv->nqubits = sv->nqubits;
    result_sv->data = new_data;
    result_sv->measurements = sv->measurements;

    return result_sv;
}

struct List *init_measurement_list(struct Statevec *sv)
{
    struct List *m = init_list(sizeof(int), 1 << sv->nqubits);
    struct List *tmp = m;
    while (tmp)
    {
        tmp->data = int_alloc(0);
        tmp = tmp->next;
    }
}

float statevec_norm(struct Statevec *sv)
{

}

struct Statevec *normalize(struct Statevec *sv)
{

}

float expectation_proj(struct Statevec *sv, enum PROJECTOR proj)
{

}

struct List *measure(struct Statevec *sv, enum PROJECTOR proj, int target)
{
    struct List *measurements = sv->measurements;
    if (!measurements)
    {
        measurements = init_measurement_list(sv);
    }

    

    
}