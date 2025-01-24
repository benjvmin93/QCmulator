#ifndef GATE_H
#define GATE_H

#include <complex.h>

enum GATE
{
    ID=0,
    X,
    Y,
    Z,
    H,
    RX,
    RY,
    RZ,
    CX,
    CZ,
    CCX,
};

struct Gate
{
    double complex *data;
    enum GATE id;
    unsigned char nqubits;
};

struct Gate *get_nqubits_from_gate_id(struct Gate* gate);
struct Gate *get_data_from_gate_id(struct Gate *gate, double *theta);
char *gate_id_to_str(enum GATE id);

struct Gate *init_gate(enum GATE id, double *alpha);
void free_gate(struct Gate *gate);
void print_gate(struct Gate *gate);

#endif