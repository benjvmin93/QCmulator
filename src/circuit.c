#include "headers/circuit.h"
#include "headers/alloc.h"
#include "headers/projector.h"

#include <stdio.h>

struct Instruction *init_instruction(enum GATE gate_id, double *theta, struct List *targets)
{   
    struct Gate *gate = NULL;
    if (gate_id == M)
    {
        gate = xmalloc(sizeof(struct Gate));
        gate->id = gate_id;
        gate->nqubits = 1;
        gate->data = get_data_from_proj_id(ZERO);
    }
    else
    {
        gate = init_gate(gate_id, theta);
        size_t ntargets = list_length(targets);
        if (gate->nqubits != ntargets)
        {
            fprintf(stderr, "Can't initialize %s gate (%d qubit gate) with %ld targets.", gate_id_to_str(gate_id), gate->nqubits, ntargets);
            free_gate(gate);
            return NULL;
        }
    }

    struct Instruction *instruction = xmalloc(sizeof(struct Instruction));
    instruction->gate = gate;
    instruction->targets = targets;

    return instruction;
}

void free_instruction(struct Instruction *instruction)
{
    free_list(instruction->targets);
    free_gate(instruction->gate);
    free(instruction);
}

struct Circuit *init_circuit(unsigned char nqubits)
{
    struct Circuit *circuit = xmalloc(sizeof(struct Circuit));
    circuit->instructions = init_list(sizeof(struct Instruction *), 0);
    circuit->nqubits = nqubits;

    return circuit;
}

void free_circuit(struct Circuit *circuit)
{
    struct List *head_instr = circuit->instructions;

    while (head_instr)
    {
        struct Instruction *instr = (struct Instruction *) head_instr->data;
        if (instr)
        {
            free_instruction(instr);
        }
        struct List *tmp = head_instr;
        head_instr = head_instr->next;
        free(tmp);
    }
    free(circuit);
}

struct Circuit *add_gate(struct Circuit *circuit, enum GATE gate_id, double *theta, struct List *targets)
{
    if (circuit == NULL)
    {
        return circuit;
    }

    struct Instruction *new_instruction = init_instruction(gate_id, theta, targets);
    circuit->instructions = list_append(circuit->instructions, new_instruction);

    return circuit;
}

struct Circuit *x(struct Circuit *c, int target)
{
    void *t[] = { int_alloc(target) };
    return add_gate(c, X, NULL, init_list_from_array(t, 1, sizeof(int)));
}
struct Circuit *y(struct Circuit *c, int target)
{
    void *t[] = { int_alloc(target) };
    return add_gate(c, Y, NULL, init_list_from_array(t, 1, sizeof(int)));
}
struct Circuit *z(struct Circuit *c, int target)
{
    void *t[] = { int_alloc(target) };
    return add_gate(c, Z, NULL, init_list_from_array(t, 1, sizeof(int)));
}
struct Circuit *h(struct Circuit *c, int target)
{
    void *t[] = { int_alloc(target) };
    return add_gate(c, H, NULL, init_list_from_array(t, 1, sizeof(int)));
}
struct Circuit *rx(struct Circuit *c, double theta, int target)
{
    void *t[] = { int_alloc(target) };
    return add_gate(c, X, &theta, init_list_from_array(t, 1, sizeof(int)));
}
struct Circuit *ry(struct Circuit *c, double theta, int target)
{
    void *t[] = { int_alloc(target) };
    return add_gate(c, X, &theta, init_list_from_array(t, 1, sizeof(int)));
}
struct Circuit *rz(struct Circuit *c, double theta, int target)
{
    void *t[] = { int_alloc(target) };
    return add_gate(c, X, &theta, init_list_from_array(t, 1, sizeof(int)));
}

struct Circuit *cx(struct Circuit *c, int q0, int q1)
{
    void *t[] = { int_alloc(q0), int_alloc(q1) };
    return add_gate(c, CX, NULL, init_list_from_array(t, 2, sizeof(int)));
}
struct Circuit *cz(struct Circuit *c, int q0, int q1)
{
    void *t[] = { int_alloc(q0), int_alloc(q1) };
    return add_gate(c, CZ, NULL, init_list_from_array(t, 2, sizeof(int)));
}

struct Circuit *ccx(struct Circuit *c, int q0, int q1, int q2)
{
    void *t[] = { int_alloc(q0), int_alloc(q1), int_alloc(q2) };
    return add_gate(c, CCX, NULL, init_list_from_array(t, 3, sizeof(int)));
}

struct Circuit *measure(struct Circuit *c, int target)
{
    if (c == NULL)
    {
        fprintf(stderr, "Error: Circuit is NULL.\n");
        return NULL;
    }

    void *t[] = { int_alloc(target) };
    return add_gate(c, M, NULL, init_list_from_array(t, 1, sizeof(int)));
}