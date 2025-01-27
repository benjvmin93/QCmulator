#include "../src/headers/statevec.h"
#include "../src/headers/gate.h"
#include "../src/headers/alloc.h"
#include "../src/headers/list.h"
#include "../src/headers/projector.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#define PI 3.14159265358979323846

void test_init_statevec_valid(void)
{
    unsigned char nqubits = 3;
    struct Statevec *sv = init_statevec(nqubits);

    CU_ASSERT_PTR_NOT_NULL(sv);
    CU_ASSERT_EQUAL(sv->nqubits, nqubits);
    CU_ASSERT_PTR_NOT_NULL(sv->data);

    size_t size = 1 << nqubits;
    for (size_t i = 0; i < size; ++i)
    {
        if (i == 0)
        {
            CU_ASSERT_DOUBLE_EQUAL(sv->data[i], 1, 0.001);
        }
        else
        {
            CU_ASSERT_DOUBLE_EQUAL(sv->data[i], 0, 0.001);
        }
    }

    free_statevec(sv);
}

void test_init_statevec_zero_qubits(void)
{
    struct Statevec *sv = init_statevec(0);
    CU_ASSERT_PTR_NULL(sv);
}

void test_free_statevec_valid(void)
{
    struct Statevec *sv = init_statevec(2);

    CU_ASSERT_PTR_NOT_NULL(sv);
    CU_ASSERT_PTR_NOT_NULL(sv->data);

    free_statevec(sv);

    // It's difficult to validate freed memory, but this ensures no crashes during free.
    CU_PASS("free_statevec executed without crashing.");
}

void test_statevec_memory_leak(void)
{
    for (unsigned char nqubits = 1; nqubits <= 10; ++nqubits)
    {
        struct Statevec *sv = init_statevec(nqubits);
        CU_ASSERT_PTR_NOT_NULL(sv);
        free_statevec(sv);
    }

    CU_PASS("No memory issues detected in repeated statevec allocation and deallocation.");
}

void test_statevec_data_access(void)
{
    for (unsigned char nqubits = 1; nqubits <= 10; ++nqubits)
    {
        struct Statevec *sv = init_statevec(nqubits);
        CU_ASSERT_PTR_NOT_NULL(sv);

        size_t size = 1 << sv->nqubits;
        for (size_t i = 0; i < size; ++i)
        {
            double complex data = sv->data[i];
            double mod = cabs(data);
        }
        free_statevec(sv);
    }
}

void test_evolve_single_X_gate(void)
{
    unsigned char nqubits = 2;
    struct Statevec *sv = init_statevec(nqubits);
    struct Gate *x_gate = init_gate(X, NULL);

    // Apply X gate to the first qubit
    sv = evolve_single(sv, x_gate->data, 0);

    // Expected result: |10> state
    CU_ASSERT_EQUAL(sv->data[0], 0);
    CU_ASSERT_EQUAL(sv->data[1], 0);
    CU_ASSERT_EQUAL(sv->data[2], 1);
    CU_ASSERT_EQUAL(sv->data[3], 0);

    free_statevec(sv);
    free_gate(x_gate);
}

void test_evolve_single_H_gate(void)
{
    unsigned char nqubits = 1;
    struct Statevec *sv = init_statevec(nqubits);
    struct Gate *h_gate = init_gate(H, NULL);

    // Apply H gate
    sv = evolve_single(sv, h_gate->data, 0);

    // Expected result: superposition |0> + |1>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[0]), sqrt(0.5), 0.001);
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[1]), sqrt(0.5), 0.001);

    free_statevec(sv);
    free_gate(h_gate);
}

void test_evolve_single_RY_gate(void)
{
    unsigned char nqubits = 1;
    struct Statevec *sv = init_statevec(nqubits);
    double theta = PI;
    struct Gate *ry_gate = init_gate(RY, &theta);

    // Apply RY gate
    sv = evolve_single(sv, ry_gate->data, 0);

    // Expected result: |1>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[0]), 0., 0.001);
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[1]), 1., 0.001);

    free_statevec(sv);
    free_gate(ry_gate);
}

void test_evolve_single_target_qubit(void)
{
    unsigned char nqubits = 3;
    struct Statevec *sv = init_statevec(nqubits);
    struct Gate *x_gate = init_gate(X, NULL);

    // Apply X gate to the last qubit
    sv = evolve_single(sv, x_gate->data, 2);

    // Verify the state vector after applying X gate
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[0]), 0.0, 0.001); // 000
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[1]), 1.0, 0.001); // 001
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[2]), 0.0, 0.001); // 010
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[3]), 0.0, 0.001); // 011
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[4]), 0.0, 0.001); // 100
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[5]), 0.0, 0.001); // 101
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[6]), 0.0, 0.001); // 110
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[7]), 0.0, 0.001); // 111

    free_statevec(sv);
    free_gate(x_gate);
}

void test_evolve_single_invalid_args(void)
{
    unsigned char nqubits = 2;
    struct Statevec *sv = init_statevec(nqubits);

    // Pass NULL as the gate
    sv = evolve_single(sv, NULL, 0);
    // Expect same sv
    CU_ASSERT_PTR_NOT_NULL(sv);
    free_statevec(sv);

    sv = NULL;
    sv = evolve_single(sv, NULL, 0);

    CU_ASSERT_PTR_NULL(sv);
}

void test_evolve_CNOT_gate_00(void)
{
    unsigned char nqubits = 2;
    struct Statevec *sv = init_statevec(nqubits);
    struct Gate *cnot_gate = init_gate(CX, NULL);

    void *values[] = {int_alloc(0), int_alloc(1)};
    struct List *targets = init_list_from_array(values, 2, sizeof(int));

    sv = evolve(sv, cnot_gate, targets);
    free_list(targets);

    // Expected: |00>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[0]), 1.0, 0.001); // |00>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[3]), 0.0, 0.001); // |11>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[1]), 0.0, 0.001); // |01>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[2]), 0.0, 0.001); // |10>

    free_statevec(sv);
    free_gate(cnot_gate);
}

void test_evolve_CNOT_gate_11(void)
{
    unsigned char nqubits = 2;

    struct Statevec *sv = init_statevec(nqubits);
    struct Gate *x_gate = init_gate(X, NULL);
    struct Gate *cnot_gate = init_gate(CX, NULL);
    
    sv = evolve_single(sv, x_gate->data, 0);
    sv = evolve_single(sv, x_gate->data, 1);
    free_gate(x_gate);    

    void *values[] = {int_alloc(1), int_alloc(0)};
    struct List *targets = init_list_from_array(values, 2, sizeof(int));
    
    sv = evolve(sv, cnot_gate, targets);
    free_gate(cnot_gate);
    free_list(targets);

    // Expected: |01>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[0]), 0.0, 0.001); // |00>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[3]), 0.0, 0.001); // |11>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[1]), 1.0, 0.001); // |01>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[2]), 0.0, 0.001); // |10>

    free_statevec(sv);
}

void test_evolve_CCNOT_gate(void)
{
    unsigned char nqubits = 3;
    struct Statevec *sv = init_statevec(nqubits);
    struct Gate *ccnot_gate = init_gate(CCX, NULL);

    void *values[] = {int_alloc(0), int_alloc(1), int_alloc(2)}; // Controls: 0, 1; Target: 2
    struct List *targets = init_list_from_array(values, 3, sizeof(int));

    sv = evolve(sv, ccnot_gate, targets);
    free_list(targets);

    // Expected: |000>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[0]), 1.0, 0.001); // |000>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[7]), 0.0, 0.001); // |111>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[3]), 0.0, 0.001); // |011>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[4]), 0.0, 0.001); // |100>

    free_gate(ccnot_gate);
    free_statevec(sv);
}

void test_evolve_Hadamard_gate(void)
{
    unsigned char nqubits = 1;
    struct Statevec *sv = init_statevec(nqubits);
    struct Gate *h_gate = init_gate(H, NULL);

    struct List *targets = init_list(sizeof(int), 1);
    list_set(targets, 0, int_alloc(0));   // Target: 0

    sv = evolve(sv, h_gate, targets);
    free_list(targets);

    // Expected: superposition |0> + |1>
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[0]), sqrt(0.5), 0.001);
    CU_ASSERT_DOUBLE_EQUAL(creal(sv->data[1]), sqrt(0.5), 0.001);

    free_statevec(sv);
    free_gate(h_gate);
}

void test_evolve_invalid_inputs(void)
{
    unsigned char nqubits = 2;
    struct Statevec *sv = init_statevec(nqubits);
    struct Gate *cnot_gate = init_gate(CX, NULL);

    void *t[] = {int_alloc(0), int_alloc(1)};
    struct List *targets = init_list_from_array(t, 2, sizeof(int));

    // Null state vector
    struct Statevec *result_sv = evolve(NULL, cnot_gate, targets);
    CU_ASSERT_PTR_NULL(result_sv);

    // Null gate
    struct Statevec *result_sv2 = evolve(sv, NULL, targets);
    CU_ASSERT_PTR_NOT_NULL(result_sv2);
    free_list(targets);

    // Null targets
    struct Statevec *result_sv3 = evolve(sv, cnot_gate, NULL);
    CU_ASSERT_PTR_NOT_NULL(result_sv3);

    free_statevec(sv);
    free_gate(cnot_gate);
}

void test_evolve_incorrect_targets(void)
{
    unsigned char nqubits = 2;
    struct Statevec *sv = init_statevec(nqubits);
    struct Gate *cnot_gate = init_gate(CX, NULL);

    void *t[] = {int_alloc(0)}; // Missing second target
    struct List *targets = init_list_from_array(t, 1, sizeof(int));

    sv = evolve(sv, cnot_gate, targets);
    CU_ASSERT_PTR_NOT_NULL(sv); // Should return sv due to target mismatch
    free_list(targets);

    free_statevec(sv);
    free_gate(cnot_gate);
}

void test_norm(void)
{
    struct Statevec *sv = xmalloc(sizeof(struct Statevec));
    sv->nqubits = 1;
    sv->data = xmalloc(2 * sizeof(double complex));
    sv->data[0] = 1. / sqrt(2);
    sv->data[1] = 1. / sqrt(2);

    double norm = statevec_norm(sv);
    CU_ASSERT_DOUBLE_EQUAL(norm, 1., 0.001);
    free(sv->data);

    sv->nqubits = 2;
    sv->data = xmalloc(4 * sizeof(double complex));
    sv->data[0] = 1;
    sv->data[1] = 1;
    sv->data[2] = 1;
    sv->data[3] = 1;

    norm = statevec_norm(sv);
    CU_ASSERT_DOUBLE_EQUAL(norm, 2., 0.001);
    free(sv->data);

    sv->nqubits = 1;
    sv->data = xmalloc(2 * sizeof(double complex));
    sv->data[0] = 0;
    sv->data[1] = 0;

    norm = statevec_norm(sv);
    CU_ASSERT_DOUBLE_EQUAL(norm, 0., 0.001);
    free(sv->data);

    free(sv);
}

void test_normalize(void)
{
    double complex data1[] = {1.0 + I, 2.0, 0.0, -5.0*I};
    struct Statevec sv = { .data = data1, .nqubits = 2 };

    struct Statevec *normalized = normalize(&sv);

    CU_ASSERT_DOUBLE_EQUAL(statevec_norm(normalized), 1., 0.001);
}

void test_expectation_proj(void)
{
    struct Statevec *sv = init_statevec(1);
    sv->data[0] = 1.;
    sv->data[1]=  0.;
    CU_ASSERT_DOUBLE_EQUAL(creal(expectation_proj(sv, ZERO, 0)), 1., 1e-6);

    // Test case 2: |psi> = |1>, projector = |0><0|
    sv->data[0] = 0.;
    sv->data[1] = 1.;
    CU_ASSERT_DOUBLE_EQUAL(creal(expectation_proj(sv, ZERO, 0)), 0., 1e-6);
    free_statevec(sv);

    
    // Test case 3: Bell state, projector = |00><00|
    sv = init_statevec(2);
    sv->data[0] = 1. / sqrt(2);
    sv->data[1] = 0.;
    sv->data[2] = 0.;
    sv->data[3] = 1. / sqrt(2);
    CU_ASSERT_DOUBLE_EQUAL(creal(expectation_proj(sv, ZERO, 0)), 0.5, 1e-6);
    free_statevec(sv);
}

void test_measure(void)
{
    // Test case 1: Measure |0>
    struct Statevec *sv = init_statevec(1);
    sv->data[0] = 1.;
    sv->data[1] = 0.;
    
    sv = measure_single(sv, 0);
    
    CU_ASSERT(*sv->measurements[0] == false);
    free_statevec(sv);

    // Test case 2: Measure |1>
    sv = init_statevec(1);
    sv->data[0] = 0.;
    sv->data[1] = 1.;

    sv = measure_single(sv, 0);
    
    CU_ASSERT(*sv->measurements[0] == true);
    free_statevec(sv);

    // Test case 3: Bell state
    sv = init_statevec(2);
    sv->data[0] = 1. / sqrt(2);
    sv->data[1] = 0.;
    sv->data[2] = 0.;
    sv->data[3] = 1. / sqrt(2);

    sv = measure_single(sv, 0);
    CU_ASSERT(sv->measurements[0] != NULL);

    sv = measure_single(sv, 1);
    CU_ASSERT(sv->measurements[1] != NULL);
    CU_ASSERT_EQUAL(*sv->measurements[0], *sv->measurements[1]);

    free_statevec(sv);
}

int main()
{
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("Statevec Tests", 0, 0);

    CU_add_test(suite, "Test Valid Initialization", test_init_statevec_valid);
    CU_add_test(suite, "Test Zero Qubits Initialization", test_init_statevec_zero_qubits);
    CU_add_test(suite, "Test Free Statevec", test_free_statevec_valid);
    CU_add_test(suite, "Test Memory Leak", test_statevec_memory_leak);
    CU_add_test(suite, "Test Data Access", test_statevec_data_access);
    CU_add_test(suite, "Test Evolve Single with X Gate", test_evolve_single_X_gate);
    CU_add_test(suite, "Test Evolve Single with H Gate", test_evolve_single_H_gate);
    CU_add_test(suite, "Test Evolve Single with RY Gate", test_evolve_single_RY_gate);
    CU_add_test(suite, "Test Evolve Single on Target Qubit", test_evolve_single_target_qubit);
    CU_add_test(suite, "Test Evolve Single with Invalid Gate", test_evolve_single_invalid_args);
    CU_add_test(suite, "Test Evolve with CNOT Gate", test_evolve_CNOT_gate_00);
    CU_add_test(suite, "Test Evolve with CNOT Gate", test_evolve_CNOT_gate_11);
    CU_add_test(suite, "Test Evolve with CCNOT Gate", test_evolve_CCNOT_gate);
    CU_add_test(suite, "Test Evolve with Hadamard Gate", test_evolve_Hadamard_gate);
    CU_add_test(suite, "Test Evolve with Invalid Inputs", test_evolve_invalid_inputs);
    CU_add_test(suite, "Test Evolve with Incorrect Targets", test_evolve_incorrect_targets);
    CU_add_test(suite, "Test Norm", test_norm);
    CU_add_test(suite, "Test Normalize", test_normalize);
    CU_add_test(suite, "Test Expectation Proj", test_expectation_proj);
    CU_add_test(suite, "Test Measure", test_measure);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return 0;
}
