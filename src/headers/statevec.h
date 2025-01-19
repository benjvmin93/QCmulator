#ifndef STATEVEC_H
#define STATEVEC_H

#include "gate.h"
#include "list.h"

#include <complex.h>
#include <stdbool.h>

struct Statevec
{
    double complex *data;
    bool **measurements;
    unsigned char nqubits;
};

struct Statevec *init_statevec(unsigned char nqubits);
void free_statevec(struct Statevec *sv);
double statevec_norm(struct Statevec *sv);
struct Statevec *normalize(struct Statevec *sv);
struct Statevec *evolve_single(struct Statevec *sv, double complex *gate, unsigned char target);
struct Statevec *evolve(struct Statevec *sv, struct Gate *gate, struct List *targets);
void print_statevec(struct Statevec *sv);

#endif