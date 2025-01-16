#ifndef GATE_H
#define GATE_H

enum GATE
{
    I=0,
    X,
    Y,
    Z,
    H,
    RX,
    RY,
    RZ,
    CX,
    CZ,
};

struct Gate
{
    struct Complex **data;
    unsigned char nqubits;
};

struct Gate *get_nqubits_from_gate_id(struct Gate* gate, enum GATE id);
struct Gate *get_data_from_gate_id(struct Gate *gate, enum GATE id, double *theta);

struct Gate *init_gate(enum GATE id, double *alpha);
void free_gate(struct Gate *gate);

#endif