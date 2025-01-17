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
    enum GATE id;
    unsigned char nqubits;
};

struct Gate *get_nqubits_from_gate_id(struct Gate* gate);
struct Gate *get_data_from_gate_id(struct Gate *gate, double *theta);

struct Gate *init_gate(enum GATE id, double *alpha);
void free_gate(struct Gate *gate);
void print_gate(struct Gate *gate);

#endif