#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../src/headers/complex.h"

void test_init_complex(void)
{
    struct Complex *z = init_complex(3.0, 4.0);
    CU_ASSERT_PTR_NOT_NULL(z);
    CU_ASSERT_PTR_NOT_NULL(z->a);
    CU_ASSERT_PTR_NOT_NULL(z->b);
    CU_ASSERT_DOUBLE_EQUAL(*z->a, 3.0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(*z->b, 4.0, 0.001);
    free_complex(z);
}

void test_complex_add(void)
{
    struct Complex *z1 = init_complex(1.0, 2.0);
    struct Complex *z2 = init_complex(3.0, 4.0);
    struct Complex *result = complex_add(z1, z2);

    CU_ASSERT_PTR_NOT_NULL(result);
    CU_ASSERT_DOUBLE_EQUAL(*result->a, 4.0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(*result->b, 6.0, 0.001);

    free_complex(z1);
    free_complex(z2);
    free_complex(result);
}

void test_complex_mul(void)
{
    struct Complex *z1 = init_complex(1.0, 2.0);
    struct Complex *z2 = init_complex(3.0, 4.0);
    struct Complex *result = complex_mul(z1, z2);

    CU_ASSERT_PTR_NOT_NULL(result);
    CU_ASSERT_DOUBLE_EQUAL(*result->a, -5.0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(*result->b, 10.0, 0.001);

    free_complex(z1);
    free_complex(z2);
    free_complex(result);
}

void test_complex_div(void)
{
    struct Complex *z1 = init_complex(1.0, 2.0);
    struct Complex *z2 = init_complex(3.0, 4.0);
    struct Complex *result = complex_div(z1, z2);

    CU_ASSERT_PTR_NOT_NULL(result);
    CU_ASSERT_DOUBLE_EQUAL(*result->a, 0.44, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(*result->b, 0.08, 0.001);

    free_complex(z1);
    free_complex(z2);
    free_complex(result);
}

void test_complex_div_by_zero(void)
{
    struct Complex *z1 = init_complex(1.0, 2.0);
    struct Complex *z2 = init_complex(0.0, 0.0);
    struct Complex *result = complex_div(z1, z2);

    CU_ASSERT_PTR_NULL(result);

    free_complex(z1);
    free_complex(z2);
}

void test_complex_modulus(void)
{
    struct Complex *z = init_complex(3.0, 4.0);
    double modulus = complex_modulus(z);

    CU_ASSERT_DOUBLE_EQUAL(modulus, 5.0, 0.001);
    free_complex(z);
}

void test_complex_equal(void)
{
    struct Complex *z1 = init_complex(1.0, 2.0);
    struct Complex *z2 = init_complex(1.0, 2.01);
    struct Complex *z3 = init_complex(1.0, 2.0);

    CU_ASSERT_TRUE(complex_equal(z1, z3, 0.01));
    CU_ASSERT_FALSE(complex_equal(z1, z2, 0.001));

    free_complex(z1);
    free_complex(z2);
    free_complex(z3);
}

void test_complex_zero_operations(void)
{
    struct Complex *z1 = init_complex(0.0, 0.0);
    struct Complex *z2 = init_complex(1.0, 1.0);

    struct Complex *add_result = complex_add(z1, z2);
    struct Complex *mul_result = complex_mul(z1, z2);
    struct Complex *div_result = complex_div(z2, z1);

    CU_ASSERT_PTR_NOT_NULL(add_result);
    CU_ASSERT_DOUBLE_EQUAL(*add_result->a, 1.0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(*add_result->b, 1.0, 0.001);

    CU_ASSERT_PTR_NOT_NULL(mul_result);
    CU_ASSERT_DOUBLE_EQUAL(*mul_result->a, 0.0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(*mul_result->b, 0.0, 0.001);

    CU_ASSERT_PTR_NULL(div_result); // Division by zero should return NULL.

    free_complex(z1);
    free_complex(z2);
    free_complex(add_result);
    free_complex(mul_result);
}

void test_complex_negative_numbers(void)
{
    struct Complex *z1 = init_complex(-1.0, -2.0);
    struct Complex *z2 = init_complex(3.0, 4.0);
    struct Complex *result = complex_add(z1, z2);

    CU_ASSERT_PTR_NOT_NULL(result);
    CU_ASSERT_DOUBLE_EQUAL(*result->a, 2.0, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(*result->b, 2.0, 0.001);

    free_complex(z1);
    free_complex(z2);
    free_complex(result);
}

int main()
{
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("Complex Tests", 0, 0);

    CU_add_test(suite, "Test Initialization", test_init_complex);
    CU_add_test(suite, "Test Addition", test_complex_add);
    CU_add_test(suite, "Test Multiplication", test_complex_mul);
    CU_add_test(suite, "Test Division", test_complex_div);
    CU_add_test(suite, "Test Division by Zero", test_complex_div_by_zero);
    CU_add_test(suite, "Test Modulus", test_complex_modulus);
    CU_add_test(suite, "Test Equality", test_complex_equal);
    CU_add_test(suite, "Test Zero Operations", test_complex_zero_operations);
    CU_add_test(suite, "Test Negative Numbers", test_complex_negative_numbers);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return 0;
}
