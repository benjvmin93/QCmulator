#include "headers/circuit.h"
#include "headers/alloc.h"

#include <stdio.h>

struct Instruction *init_instruction(enum GATE gate_id, double *theta, struct List *targets)
{
    struct Gate *gate = init_gate(gate_id, theta);
    size_t ntargets = list_length(targets);
    if (gate->nqubits != ntargets)
    {
        fprintf(stderr, "Can't initialize %s gate (%d qubit gate) with %ld targets.", gate_id_to_str(gate_id), gate->nqubits, ntargets);
        free_gate(gate);
        return NULL;
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

    size_t ntargets = list_size(targets);
    struct Instruction *new_instruction = init_instruction(gate_id, theta, targets);
    circuit->instructions = list_append(circuit->instructions, new_instruction);

    return circuit;
}