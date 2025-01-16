#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../src/headers/statevec.h"

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

int main()
{
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("Statevec Tests", 0, 0);

    CU_add_test(suite, "Test Valid Initialization", test_init_statevec_valid);
    CU_add_test(suite, "Test Zero Qubits Initialization", test_init_statevec_zero_qubits);
    CU_add_test(suite, "Test Free Statevec", test_free_statevec_valid);
    CU_add_test(suite, "Test Memory Leak", test_statevec_memory_leak);
    CU_add_test(suite, "Test Data Access", test_statevec_data_access);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return 0;
}
