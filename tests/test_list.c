#include "../src/headers/list.h"
#include "../src/headers/alloc.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include <stdlib.h>
#include <string.h>

// Helper function to compare two arrays
int compare_arrays(const void *a, const void *b, size_t size) {
    return memcmp(a, b, size);
}

// Test initialization of the list
void test_init_list(void) {
    size_t data_size = sizeof(int);
    size_t initial_size = 5;

    struct List *list = init_list(data_size, initial_size);
    CU_ASSERT_PTR_NOT_NULL(list);
    CU_ASSERT_EQUAL(list_size(list), initial_size);

    free_list(list);
}

// Test appending data to the list
void test_list_append(void) {
    struct List *list = init_list(sizeof(int), 0);

    int *values[] = {int_alloc(10), int_alloc(20), int_alloc(30)};
    
    for (size_t i = 0; i < 3; ++i) {
        list = list_append(list, values[i]);
    }

    CU_ASSERT_EQUAL(list_size(list), 3);

    // Verify appended values
    for (size_t i = 0; i < 3; ++i) {
        struct List *node = list_get(list, i);
        CU_ASSERT_PTR_NOT_NULL(node);
        CU_ASSERT_EQUAL(*(int *)node->data, *values[i]);
    }

    free_list(list);
}

// Test setting data in the list
void test_list_set(void) {
    struct List *list = init_list(sizeof(int), 3);
    int *new_value = int_alloc(42);

    list_set(list, 1, new_value);
    struct List *node = list_get(list, 1);

    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(*(int *)node->data, *new_value);

    free_list(list);
}

// Test getting data from the list
void test_list_get(void) {
    struct List *list = init_list(sizeof(int), 3);
    int *values[] = {int_alloc(1), int_alloc(2), int_alloc(3)};

    for (size_t i = 0; i < 3; ++i) {
        list_set(list, i, values[i]);
    }

    for (size_t i = 0; i < 3; ++i) {
        struct List *node = list_get(list, i);
        CU_ASSERT_PTR_NOT_NULL(node);
        CU_ASSERT_EQUAL(*(int *)node->data, *values[i]);
    }

    free_list(list);
}

// Test freeing the list
void test_free_list(void) {
    struct List *list = init_list(sizeof(int), 5);
    free_list(list);

    // If free_list executes without crashing, the test passes
    CU_PASS("free_list executed successfully.");
}

// Test list size calculation
void test_list_size(void) {
    struct List *list = init_list(sizeof(int), 0);

    
    for (int i = 0; i < 10; ++i) {
        int *value = int_alloc(5);
        list = list_append(list, value);
    }

    CU_ASSERT_EQUAL(list_size(list), 10);

    free_list(list);
}

// Test list size calculation
void test_list_from_array(void) {
    void *array[] = { int_alloc(0), int_alloc(1), int_alloc(2), int_alloc(3) };
    struct List *list = init_list_from_array(array, 4, sizeof(int));
    CU_ASSERT_EQUAL(list_size(list), 4);
    for (int i = 0; i < 4; ++i) {
        CU_ASSERT_EQUAL(*(int*)list_get(list, i)->data, i);
    }

    free_list(list);
}

int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("List Tests", 0, 0);
    CU_ADD_TEST(suite, test_init_list);
    CU_ADD_TEST(suite, test_list_append);
    CU_ADD_TEST(suite, test_list_set);
    CU_ADD_TEST(suite, test_list_get);
    CU_ADD_TEST(suite, test_free_list);
    CU_ADD_TEST(suite, test_list_size);
    CU_ADD_TEST(suite, test_list_from_array);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return 0;
}
