#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "gate.h"
#include "list.h"

struct Instruction
{
    struct Gate *gate;
    struct List *targets;
};

struct Instruction *init_instruction(enum GATE gate_id, double *theta, struct List *targets);
void free_instruction(struct Instruction *instruction);

struct Circuit
{
    struct List *instructions;
    unsigned char nqubits;
};

struct Circuit *init_circuit(unsigned char nqubits);
void free_circuit(struct Circuit *circuit);
struct Circuit *add_gate(struct Circuit *circuit, enum GATE gate_id, double *theta, struct List *targets);

struct Circuit *x(struct Circuit *c, int target);
struct Circuit *y(struct Circuit *c, int target);
struct Circuit *z(struct Circuit *c, int target);
struct Circuit *h(struct Circuit *c, int target);
struct Circuit *rx(struct Circuit *c, double theta, int target);
struct Circuit *ry(struct Circuit *c, double theta, int target);
struct Circuit *rz(struct Circuit *c, double theta, int target);

struct Circuit *cx(struct Circuit *c, int q0, int t);
struct Circuit *cz(struct Circuit *c, int q0, int q2);
struct Circuit *ccx(struct Circuit *c, int q0, int q1, int q2);

#endif