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

#endif