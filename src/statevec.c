#include "headers/statevec.h"
#include "headers/alloc.h"
#include "headers/gate.h"
#include "headers/projector.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void print_statevec(struct Statevec *sv)
{
    printf("\n[");
    size_t dim = 1 << sv->nqubits;
    for (size_t idx = 0; idx < dim; ++idx)
    {
        if (idx < dim - 1)
        {
            printf("\t%.10f%+.10fi\n", creal(sv->data[idx]), cimag(sv->data[idx]));
        }
        else
        {
            printf("\t%.10f%+.10fi]\n", creal(sv->data[idx]), cimag(sv->data[idx]));
        }
        
    }
}

void printBinary(int num)
{
    for (int i = sizeof(int) * 8 - 1; i >= 0; i--)
    {
        printf("%d", (num >> i) & 1);
    }
}

struct Statevec *init_statevec(unsigned char nqubits)
{
    if (nqubits == 0)
    {
        return NULL;
    }

    struct Statevec *sv = xmalloc(sizeof(struct Statevec));
    size_t size = 1 << nqubits;
    
    sv->nqubits = nqubits;
    sv->measurements = xcalloc(sizeof(bool *), nqubits);
    sv->data = xcalloc(sizeof(double complex), size);
    sv->data[0] = 1;

    return sv;
}

void free_statevec(struct Statevec *sv)
{
    for (size_t i = 0; i < sv->nqubits; ++i)
    {
        if (sv->measurements[i])
        {
            free(sv->measurements[i]);
        }
    }
    free(sv->measurements);
    free(sv->data);
    free(sv);
}

struct Statevec *evolve_single(struct Statevec *sv, double complex *gate, unsigned char target)
{
    if (gate == NULL || sv == NULL)
    {
        return sv;
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

        new_data[i] = gate[index_op_high] * sv->data[index_cell0] + gate[index_op_high | 1] * sv->data[index_cell1];
    }
    free(sv->data);
    sv->data = new_data;

    return sv;
}

struct Statevec *evolve(struct Statevec *sv, struct Gate *gate, struct List *targets)
{
    if (gate == NULL || sv == NULL || targets == NULL)
    {
        return sv;
    }

    unsigned char nqubits_sv = sv->nqubits;
    unsigned char nqubits_gate = gate->nqubits;
    size_t nTargets = list_size(targets);

    if (nqubits_gate != nTargets)
    {
        fprintf(stderr, "The number of targets is not consistant with the gate.\n");
        print_gate(gate);
        return sv;
    }

    int mask = 0;

    struct List *tmp_targets = targets;
    while (tmp_targets)
    {
        int *target = tmp_targets->data;
        mask |= (1 << ((int) nqubits_sv - *target - 1));
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
            for (size_t t = 0; t < nTargets; ++t)
            {
                int bit = (j >> (nTargets - t - 1)) & 1;
                struct List *list_element = list_get(targets, t);
                int *target = list_element->data;
                int bitmask = bit << (nqubits_sv - *target - 1);
                maskJ |= bitmask;
            }
            int index = i_base | maskJ;
            int rowIndex = 0;
            for (size_t t = 0; t < nTargets; ++t)
            {
                struct List *list_element = list_get(targets, t);
                int *target = list_element->data;
                int bit = (i >> (nqubits_sv - *target - 1)) & 1;
                bit = bit << (nTargets - t - 1);
                rowIndex |= bit;
            }

            int colIndex = j;

            int data_index = rowIndex * (1 << nqubits_gate) + colIndex; // Debug
            sum += gate->data[data_index] * sv->data[index];
        }

        new_data[i] = sum;
    }
    free(sv->data);
    sv->data = new_data;

    return sv;
}

double statevec_norm(struct Statevec *sv)
{
    size_t size = 1 << sv->nqubits;
    double complex *data = sv->data;

    double norm = 0.0;

    for (size_t i = 0; i < size; ++i) {
        norm += creal(data[i]) * creal(data[i]) + cimag(data[i]) * cimag(data[i]);
    }

    return sqrt(norm);
}

struct Statevec *normalize(struct Statevec *sv)
{
    double norm = statevec_norm(sv);
    size_t size = 1 << sv->nqubits;

    for (size_t i = 0; i < size; ++i)
    {
        sv->data[i] /= norm;
    }

    return sv;
}

struct Statevec *copy_statevec(struct Statevec *sv)
{
    struct Statevec *sv_copy = init_statevec(sv->nqubits);
    for (int i = 0; i < 1 << sv->nqubits; ++i)
    {
        sv_copy->data[i] = sv->data[i];
    }

    for (int i = 0; i < sv->nqubits; ++i)
    {
        if (sv->measurements[i] != NULL)
        {
            sv_copy->measurements[i] = bool_alloc(*sv->measurements[i]);
        }
    }

    return sv_copy;
}

double complex expectation_proj(struct Statevec *sv, enum PROJECTOR proj, int target)
{
    sv = normalize(sv);

    struct Statevec *projected = copy_statevec(sv);
    double complex *p = get_data_from_proj_id(proj);
    projected = evolve_single(projected, p, target);
    free(p);

    double complex res = 0;
    for (int i = 0; i < 1 << sv->nqubits; ++i)
    {
        res += conj(sv->data[i]) * projected->data[i];
    }

    free_statevec(projected);
    return res;
}

double drand(double low, double high)
{
    srand(time(NULL));
    return ((double)rand() * (high - low)) / (double)RAND_MAX + low;
}

void print_measurements(int **measurements, unsigned char nqubits)
{
    size_t size = 1 << nqubits;
    for (size_t i = 0; i < size; ++i)
    {
        printBinary(i);
        if (!measurements[i])
        {
            printf(": NULL\n");
        }
        else
        {
            printf(": %d\n", *measurements[i]);
        }

    }
}

struct Statevec *measure_single(struct Statevec *sv, int target)
{
    if (sv->measurements[target] != NULL)
    {
        fprintf(stdout, "Qubit %d already measured.\n", target);
        return sv;
    }
    enum PROJECTOR proj = ZERO;
    double prob = cabs(expectation_proj(sv, proj, target));
    double rand = drand(0., 1.);
    
    if (rand > prob)
    {
        proj = ONE;
    }

    struct Projector *p = init_projector(proj);
    sv = evolve_single(sv, p->data, target);
    free_projector(p);

    sv->measurements[target] = bool_alloc(proj == ONE ? true : false);

    return sv;
}

struct Statevec *run_from_circuit(struct Circuit *circuit)
{
    struct Statevec *sv = init_statevec(circuit->nqubits);
    printf("Initial statevec:\n");
    print_statevec(sv);
    printf("===========================\n");
    if (!sv)
    {
        return NULL;
    }
    struct List *instr = circuit->instructions;
    while (instr)
    {
        struct Instruction *instructions = instr->data;
        struct Gate *instr_gate = instructions->gate;
        if (instr_gate->id >= ID && instr_gate->id <= RZ)
        {
            unsigned char *target = instructions->targets->data;
            printf("Evolve single(%s: %d)\n", gate_id_to_str(instr_gate->id), *target);
            sv = evolve_single(sv, instr_gate->data, *target);
        }
        if (instr_gate->id >= CX && instr_gate->id <= CCX)
        {
            printf("Evolve(%s: %d%d)\n", gate_id_to_str(instr_gate->id), *(int*)instructions->targets->data, *(int*)instructions->targets->next->data);
            sv = evolve(sv, instructions->gate, instructions->targets);
        }
        if (instr_gate->id == M)
        {
            unsigned char *target = instructions->targets->data;
            printf("Measure(%d)\n", *target);
            sv = measure_single(sv, *target);
            sv = normalize(sv);
            printf("\tResult: %d", (*sv->measurements[*target] == true ? 1 : 0));
        }
        instr = instr->next;
        print_statevec(sv);
        printf("===========================\n");
    }

    return sv;
}

int **simulate_circuit(struct Circuit *circuit, int shots)
{   
    size_t size = 1 << circuit->nqubits;
    int **counts = xcalloc(sizeof(int *), size);
    while (shots-- > 0)
    {   
        struct Statevec *sv = run_from_circuit(circuit);
        int *result = xcalloc(sizeof(int), 1);
        for (size_t i = 0; i < sv->nqubits; ++i)
        {
            if (!sv->measurements[i])
            {
                continue;
            }
            int bit_result = *sv->measurements[i] == true ? 1 : 0;
            *result += bit_result << (sv->nqubits - i - 1);
        }

        if (!counts[*result])
        {
            counts[*result] = int_alloc(0);
        }

        *counts[*result] += 1;
        free(result);
        free_statevec(sv);
    }

    return counts;
}