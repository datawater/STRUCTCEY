#include "vec_tests.h"

#include <stdio.h>
#include <stdlib.h>

#include "../src/vec.h"

#define ASSERT_VEC_SIZE(vec, expected_size)                                               \
    do {                                                                                  \
        size_t actual_size = vec_size(vec);                                               \
        if (actual_size != expected_size) {                                               \
            fprintf(stderr, "Expected size: %u, got: %zu\n", expected_size, actual_size); \
            return new_result_error("Size mismatch");                                     \
        }                                                                                 \
    } while (0)

#define ASSERT_VEC_CAPACITY(vec, expected_capacity)                                                   \
    do {                                                                                              \
        size_t actual_capacity = vec_capacity(vec);                                                   \
        if (actual_capacity != expected_capacity) {                                                   \
            fprintf(stderr, "Expected capacity: %u, got: %zu\n", expected_capacity, actual_capacity); \
            return new_result_error("Capacity mismatch");                                             \
        }                                                                                             \
    } while (0)

#define ASSERT(n)                                                       \
    do {                                                                \
        char* x = (char*)safe_malloc(sizeof(char) * 64);                \
        memset(x, 0, sizeof(char) * 64);                                \
        snprintf(x, 64, "assertion failed. %s:%d", __func__, __LINE__); \
        if (!(n)) return new_result_error(x);                           \
    } while (0)

result_t test_vec_creation(void) {
    vec_t vec = vec_new(int);

    // Assert initial size and capacity
    ASSERT_VEC_SIZE(&vec, 0);
    ASSERT_VEC_CAPACITY(&vec, 1);

    return new_result_ok();
}

typedef struct {
    u64 a;
    u64 b;
    u64 c;
    u64 d;
} s_32bytes;

result_t test_vec_push_back(void) {
    {
        vec_t vec = vec_new(int);

        int val1 = 10, val2 = 20;

        vec_push_back(&vec, &val1);
        ASSERT_VEC_SIZE(&vec, 1);
        ASSERT_VEC_CAPACITY(&vec, 1);

        vec_push_back(&vec, &val2);
        ASSERT_VEC_SIZE(&vec, 2);
        ASSERT_VEC_CAPACITY(&vec, 2);
    }

    {
        vec_t vec = vec_new(s_32bytes);

        s_32bytes val1 = {10, 10, 10, 10}, val2 = {20, 20, 20, 20};

        vec_push_back(&vec, &val1);
        ASSERT_VEC_SIZE(&vec, 1);
        ASSERT_VEC_CAPACITY(&vec, 1);

        vec_push_back(&vec, &val2);
        ASSERT_VEC_SIZE(&vec, 2);
        ASSERT_VEC_CAPACITY(&vec, 2);
    }

    return new_result_ok();
}

result_t test_vec_pop_back(void) {
    vec_t vec = vec_new(int);

    int val1 = 10, val2 = 20;
    vec_push_back(&vec, &val1);
    vec_push_back(&vec, &val2);

    int* popped_value = (int*)vec_pop_back(&vec);
    ASSERT(*popped_value == val2);

    ASSERT_VEC_SIZE(&vec, 1);

    popped_value = (int*)vec_pop_back(&vec);
    ASSERT(*popped_value == val1);

    ASSERT_VEC_SIZE(&vec, 0);

    return new_result_ok();
}

result_t test_vec_at(void) {
    vec_t vec = vec_new(int);

    int val1 = 10, val2 = 20, val3 = 30;
    vec_push_back(&vec, &val1);
    vec_push_back(&vec, &val2);
    vec_push_back(&vec, &val3);

    int* retrieved_val = (int*)vec_at(&vec, 1);
    ASSERT(*retrieved_val == val2);

    return new_result_ok();
}

result_t test_vec_reserve(void) {
    vec_t vec = vec_new(int);
    int val = 100;

    vec_push_back(&vec, &val);
    ASSERT_VEC_CAPACITY(&vec, 1);  // Assuming initial capacity is 1

    vec_reserve(&vec, 10);  // Reserve capacity for 10 elements
    ASSERT_VEC_CAPACITY(&vec, 10);

    return new_result_ok();
}

result_t test_vec_shrink_to_fit(void) {
    vec_t vec = vec_new(int);
    int val = 10;

    vec_push_back(&vec, &val);
    vec_push_back(&vec, &val);

    vec_reserve(&vec, 10);  // Reserve capacity for 10 elements
    ASSERT_VEC_CAPACITY(&vec, 10);

    vec_shrink_to_fit(&vec);       // Shrink to fit the actual size
    ASSERT_VEC_CAPACITY(&vec, 2);  // Capacity should now be equal to size (2)

    return new_result_ok();
}

result_t test_vec_clear(void) {
    vec_t vec = vec_new(int);

    int val = 10;
    vec_push_back(&vec, &val);
    ASSERT_VEC_SIZE(&vec, 1);

    vec_clear(&vec);
    ASSERT_VEC_SIZE(&vec, 0);

    return new_result_ok();
}

result_t test_vec_is_empty(void) {
    vec_t vec = vec_new(int);

    ASSERT(vec_is_empty(&vec));  // Vector should be empty initially

    int val = 10;
    vec_push_back(&vec, &val);

    ASSERT(!vec_is_empty(&vec));  // Vector should not be empty after insertion

    return new_result_ok();
}

result_t test_vec_from_array(void) {
    int arr[] = {1, 2, 3, 4, 5};
    vec_t vec = vec_from_array(sizeof(int), 5, arr);

    ASSERT_VEC_SIZE(&vec, 5);
    ASSERT_VEC_CAPACITY(&vec, 5);

    int* first_elem = (int*)vec_at(&vec, 0);
    ASSERT(*first_elem == 1);

    return new_result_ok();
}
