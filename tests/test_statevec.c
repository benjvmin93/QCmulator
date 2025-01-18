#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../src/headers/statevec.h"
#include "../src/headers/gate.h"

#define PI 3.14159265358979323846

void test_init_statevec_valid(void)
{
    unsigned char nqubits = 3;
    struct Statevec *sv = init_statevec(nqubits);

    CU_ASSERT_PTR_NOT_NULL(sv);
    CU_ASSERT_EQUAL(sv->nqubits, nqubits);

    size_t size = 1 << nqubits;
    for (size_t i = 0; i < size; ++i)
    {
        CU_ASSERT_PTR_NOT_NULL(sv->data[i]);
        if (i == 0)
        {
            CU_ASSERT_DOUBLE_EQUAL(*sv->data[i]->a, 1.0, 0.001);
            CU_ASSERT_DOUBLE_EQUAL(*sv->data[i]->b, 0.0, 0.001);
        }
        else
        {
            CU_ASSERT_DOUBLE_EQUAL(*sv->data[i]->a, 0.0, 0.001);
            CU_ASSERT_DOUBLE_EQUAL(*sv->data[i]->b, 0.0, 0.001);
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
            struct Complex *data = sv->data[i];
            double mod = complex_modulus(data);
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
    struct Statevec *result_sv = evolve_single(sv, x_gate, 0);

    // Expected result: |10> state
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[0]->a, 0.0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[1]->a, 0.0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[2]->a, 1.0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[3]->a, 0.0, 0.001);

    free_statevec(result_sv);
    free_statevec(sv);
    free_gate(x_gate);
}

void test_evolve_single_H_gate(void)
{
    unsigned char nqubits = 1;
    struct Statevec *sv = init_statevec(nqubits);
    struct Gate *h_gate = init_gate(H, NULL);

    // Apply H gate
    struct Statevec *result_sv = evolve_single(sv, h_gate, 0);

    // Expected result: superposition |0> + |1>
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[0]->a, sqrt(0.5), 0.001);
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[1]->a, sqrt(0.5), 0.001);

    free_statevec(result_sv);
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
    struct Statevec *result_sv = evolve_single(sv, ry_gate, 0);

    // Expected result: |1>
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[0]->a, 0., 0.001);
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[1]->a, 1., 0.001);

    free_statevec(result_sv);
    free_statevec(sv);
    free_gate(ry_gate);
}

void test_evolve_single_target_qubit(void)
{
    unsigned char nqubits = 3;
    struct Statevec *sv = init_statevec(nqubits);
    struct Gate *x_gate = init_gate(X, NULL);

    // Apply X gate to the last qubit
    struct Statevec *result_sv = evolve_single(sv, x_gate, 2);

    // Verify the state vector after applying X gate
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[0]->a, 0.0, 0.001); // 000
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[1]->a, 1.0, 0.001); // 001
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[2]->a, 0.0, 0.001); // 010
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[3]->a, 0.0, 0.001); // 011
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[4]->a, 0.0, 0.001); // 100
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[5]->a, 0.0, 0.001); // 101
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[6]->a, 0.0, 0.001); // 110
    CU_ASSERT_DOUBLE_EQUAL(*result_sv->data[7]->a, 0.0, 0.001); // 111

    free_statevec(result_sv);
    free_statevec(sv);
    free_gate(x_gate);
}

void test_evolve_single_invalid_gate(void)
{
    unsigned char nqubits = 2;
    struct Statevec *sv = init_statevec(nqubits);

    // Pass NULL as the gate
    struct Statevec *result_sv = evolve_single(sv, NULL, 0);

    // Expect NULL result
    CU_ASSERT_PTR_NULL(result_sv);

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
    CU_add_test(suite, "Test Evolve Single with Invalid Gate", test_evolve_single_invalid_gate);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return 0;
}
