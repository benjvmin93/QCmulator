#include <CUnit/Basic.h>
#include "../src/headers/circuit.h"
#include "../src/headers/alloc.h"
#include "../src/headers/list.h"
#include "../src/headers/gate.h"

// Test for `init_instruction`
void test_init_instruction()
{   
    void *t[] = { int_alloc(0) };
    struct List *targets = init_list_from_array(t, 1, sizeof(int));

    double theta[1] = {3.14}; // Example angle
    struct Instruction *instruction = init_instruction(RX, theta, targets);

    CU_ASSERT_PTR_NOT_NULL(instruction);
    CU_ASSERT_PTR_NOT_NULL(instruction->gate);
    CU_ASSERT_EQUAL(instruction->gate->id, RX);
    CU_ASSERT_PTR_EQUAL(instruction->targets, targets);

    free_instruction(instruction);
}

// Test for `init_circuit`
void test_init_circuit()
{
    unsigned char nqubits = 2;
    struct Circuit *circuit = init_circuit(nqubits);

    CU_ASSERT_PTR_NOT_NULL(circuit);
    CU_ASSERT_EQUAL(circuit->nqubits, nqubits);
    CU_ASSERT_EQUAL(list_length(circuit->instructions), 0);

    free_circuit(circuit);
}

// Test for `add_gate`
void test_add_gate()
{
    unsigned char nqubits = 2;
    struct Circuit *circuit = init_circuit(nqubits);

    void *t[] = { int_alloc(0) };
    struct List *targets = init_list_from_array(t, 1, sizeof(int));
    double theta[1] = {1.57};
    circuit = add_gate(circuit, RX, theta, targets);

    CU_ASSERT_PTR_NOT_NULL(circuit);
    CU_ASSERT_EQUAL(list_length(circuit->instructions), 1);

    struct Instruction *instruction = (struct Instruction *)list_get(circuit->instructions, 0)->data;
    CU_ASSERT_PTR_NOT_NULL(instruction);
    CU_ASSERT_EQUAL(instruction->gate->id, RX);
    CU_ASSERT_EQUAL(list_length(instruction->targets), 1);

    free_circuit(circuit); // Frees all instructions as well
}

// Test for invalid gate
void test_invalid_gate()
{
    unsigned char nqubits = 2;
    struct Circuit *circuit = init_circuit(nqubits);

    void *t[] = { int_alloc(0) };
    struct List *targets = init_list_from_array(t, 1, sizeof(int));
    double theta[1] = {1.57};
    struct Instruction *instruction = init_instruction(CX, theta, targets); // CNOT requires 2 qubits

    CU_ASSERT_PTR_NULL(instruction);

    free_circuit(circuit);
    free_list(targets);
}

void test_single_qubit_gates()
{
    struct Circuit *circuit = init_circuit(2);

    circuit = x(circuit, 0);
    CU_ASSERT_EQUAL(list_length(circuit->instructions), 1);
    struct Instruction *instr = (struct Instruction *)list_get(circuit->instructions, 0)->data;
    CU_ASSERT_PTR_NOT_NULL(instr);
    CU_ASSERT_EQUAL(instr->gate->id, X);
    CU_ASSERT_EQUAL(list_length(instr->targets), 1);
    CU_ASSERT_EQUAL(*(int *)list_get(instr->targets, 0), 0);

    circuit = h(circuit, 1);
    CU_ASSERT_EQUAL(list_length(circuit->instructions), 2);
    instr = (struct Instruction *)list_get(circuit->instructions, 1)->data;
    CU_ASSERT_EQUAL(instr->gate->id, H);
    CU_ASSERT_EQUAL(*(int *)list_get(instr->targets, 0)->data, 1);

    free_circuit(circuit);
}

void test_parametrized_gates()
{
    struct Circuit *circuit = init_circuit(2);

    double theta = 3.14; // Example parameter
    circuit = rx(circuit, theta, 0);
    CU_ASSERT_EQUAL(list_size(circuit->instructions), 1);
    struct Instruction *instr = (struct Instruction *)list_get(circuit->instructions, 0)->data;
    CU_ASSERT_EQUAL(instr->gate->id, X);
    CU_ASSERT_EQUAL(list_size(instr->targets), 1);
    CU_ASSERT_EQUAL(*(int *)list_get(instr->targets, 0)->data, 0);

    circuit = rz(circuit, theta, 1);
    CU_ASSERT_EQUAL(list_size(circuit->instructions), 2);
    instr = (struct Instruction *)list_get(circuit->instructions, 1)->data;
    CU_ASSERT_EQUAL(instr->gate->id, X);
    CU_ASSERT_EQUAL(list_size(instr->targets), 1);
    CU_ASSERT_EQUAL(*(int *)list_get(instr->targets, 0)->data, 1);

    free_circuit(circuit);
}

void test_multi_qubit_gates()
{
    struct Circuit *circuit = init_circuit(3);

    circuit = cx(circuit, 0, 1);
    CU_ASSERT_EQUAL(list_size(circuit->instructions), 1);
    struct Instruction *instr = (struct Instruction *)list_get(circuit->instructions, 0)->data;
    CU_ASSERT_EQUAL(instr->gate->id, CX);
    CU_ASSERT_EQUAL(list_size(instr->targets), 2);
    CU_ASSERT_EQUAL(*(int *)list_get(instr->targets, 0)->data, 0);
    CU_ASSERT_EQUAL(*(int *)list_get(instr->targets, 1)->data, 1);

    circuit = ccx(circuit, 0, 1, 2);
    CU_ASSERT_EQUAL(list_size(circuit->instructions), 2);
    instr = (struct Instruction *)list_get(circuit->instructions, 1)->data;
    CU_ASSERT_EQUAL(instr->gate->id, CCX);
    CU_ASSERT_EQUAL(list_size(instr->targets), 3);
    CU_ASSERT_EQUAL(*(int *)list_get(instr->targets, 0)->data, 0);
    CU_ASSERT_EQUAL(*(int *)list_get(instr->targets, 1)->data, 1);
    CU_ASSERT_EQUAL(*(int *)list_get(instr->targets, 2)->data, 2);

    free_circuit(circuit);
}

int main()
{
    // Initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // Add a suite for the tests
    CU_pSuite suite = CU_add_suite("Circuit Tests", NULL, NULL);
    if (suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add tests to the suite
    if ((CU_add_test(suite, "test_init_instruction", test_init_instruction) == NULL) ||
        (CU_add_test(suite, "test_init_circuit", test_init_circuit) == NULL) ||
        (CU_add_test(suite, "test_add_gate", test_add_gate) == NULL) ||
        (CU_add_test(suite, "test_invalid_gate", test_invalid_gate) == NULL) ||
        (CU_add_test(suite, "test_single_qubit_gates", test_single_qubit_gates) == NULL) ||
        (CU_add_test(suite, "test_parametrized_gates", test_parametrized_gates) == NULL) ||
        (CU_add_test(suite, "test_multi_qubit_gates", test_multi_qubit_gates) == NULL))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run the tests using the Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    // Clean up the registry
    CU_cleanup_registry();
    return CU_get_error();
}
