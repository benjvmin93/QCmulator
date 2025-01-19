#ifndef STATEVEC_H
#define STATEVEC_H

#include "complex.h"
#include "gate.h"
#include "list.h"

struct Statevec
{
    struct Complex **data;
    unsigned char nqubits;
};

struct Statevec *init_statevec(unsigned char nqubits);
void free_statevec(struct Statevec *sv);
struct Statevec *evolve_single(struct Statevec *sv, struct Gate *gate, unsigned char target);
struct Statevec *evolve(struct Statevec *sv, struct Gate *gate, struct List *targets);

#endif