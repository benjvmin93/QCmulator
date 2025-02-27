#ifndef STATEVEC_H
#define STATEVEC_H

#include "gate.h"
#include "list.h"
#include "projector.h"
#include "circuit.h"
#include "alloc.h"

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
double complex expectation_proj(struct Statevec *sv, enum PROJECTOR proj, int target);
struct Statevec *measure_single(struct Statevec *sv, int target);
struct Statevec *evolve_single(struct Statevec *sv, double complex *gate, unsigned char target);
struct Statevec *evolve(struct Statevec *sv, struct Gate *gate, struct List *targets);

double **simulate_circuit(struct Circuit *circuit, int shots);
struct Statevec *run_from_circuit(struct Circuit *circuit);

void print_statevec(struct Statevec *sv);
void print_measurements(double **measurements, unsigned char nqubits);
void printBinary(int num, int bits);

#endif