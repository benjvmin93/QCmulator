#ifndef STATEVEC_H
#define STATEVEC_H

#include "complex.h"

struct Statevec
{
    struct Complex **data;
    unsigned char nqubits;
};

struct Statevec *init_statevec(unsigned char nqubits);
struct Statevec *free_statevec(struct Statevec *sv);

#endif