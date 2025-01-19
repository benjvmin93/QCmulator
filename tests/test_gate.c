#include "../src/headers/gate.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include <math.h>
#include <complex.h>

double COMPLEX_EQUAL_THRESHOLD = 1e-10;

// Test the `get_data_from_gate_id` function
void test_get_data_from_gate_id() {
    struct Gate *gate = init_gate(X, NULL);

    CU_ASSERT_EQUAL(gate->data[0 * 2 + 0], 0);
    CU_ASSERT_EQUAL(gate->data[0 * 2 + 1], 1);
    CU_ASSERT_EQUAL(gate->data[1 * 2 + 0], 1);
    CU_ASSERT_EQUAL(gate->data[1 * 2 + 1], 0);

    free_gate(gate);
}

// Test the `init_gate` function
void test_init_gate_H() {
    struct Gate *gate = init_gate(H, NULL);
    CU_ASSERT_EQUAL(gate->nqubits, 1);
    double complex FRAC_SQRT_2 = 1./sqrt(2.);
    CU_ASSERT_EQUAL(gate->data[0 * 2 + 0], FRAC_SQRT_2);
    CU_ASSERT_EQUAL(gate->data[0 * 2 + 1], FRAC_SQRT_2);

    free_gate(gate);
}

// Test the `free_gate` function
void test_free_gate() {
    struct Gate *gate = init_gate(Z, NULL);
    free_gate(gate);
    // No explicit assertion, but should not crash or leak memory
}

// Main function for running tests
int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Gate Tests", NULL, NULL);

    CU_add_test(suite, "Test get_data_from_gate_id", test_get_data_from_gate_id);
    CU_add_test(suite, "Test init_gate", test_init_gate_H);
    CU_add_test(suite, "Test free_gate", test_free_gate);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
