#include "vec.h"

#include <assert.h>
#include <stdint.h>

#include "../utils.h"

#ifndef VEC_INCREASE_CONST
#define VEC_INCREASE_CONST 1.618f
#endif  // VEC_INCREASE_CONST

#if __has_include(<immintrin.h>)
#include <immintrin.h>
#define SIMD_ENABLED 1
#endif

vec_t __vec_new(VE_EL_SIZE_T element_size) {
    assert(element_size != 0);  // This should be unreachable but whatever

    void* start = safe_malloc(element_size);

    return (vec_t){.__capacity = 1, .__size = 0, .__el_size = element_size, .__start = start};
}

vec_t vec_from_array(VE_EL_SIZE_T element_size, size_t element_count, void* elements) {
    vec_t t = __vec_new(element_size);
    vec_reserve(&t, element_count);

#ifndef SIMD_ENABLED
    memcpy(t.__start, elements, element_size * element_count);
#else
    char* dest_ptr = (char*)t.__start;
    char* src_ptr = (char*)elements;

    size_t total_bytes = element_size * element_count;

    size_t i = 0;
    for (; i + 31 < total_bytes; i += 32) {
        __m256i data = _mm256_loadu_si256((__m256i*)(src_ptr + i));
        _mm256_storeu_si256((__m256i*)(dest_ptr + i), data);
    }

    if (i < total_bytes) {
        memcpy(dest_ptr + i, src_ptr + i, total_bytes - i);
    }
#endif
    t.__size = element_count;

    return t;
}

void vec_unsafe_resize(vec_t* vec, size_t new_capacity) {
    vec->__capacity = new_capacity;
    vec->__start = safe_realloc(vec->__start, vec->__capacity * vec->__el_size);
}

void vec_reserve(vec_t* vec, size_t new_capacity) {
    assert(vec != NULL);

    if (new_capacity < vec->__capacity) return;

    vec_unsafe_resize(vec, new_capacity);
}

void vec_safe_check(vec_t* vec) {
    assert(vec != NULL);

    if (vec->__size + 1 <= vec->__capacity) return;

    vec->__capacity = (size_t)(((double)vec->__capacity) * VEC_INCREASE_CONST + 1.0);
    vec->__start = safe_realloc(vec->__start, vec->__capacity * vec->__el_size);
}

void* vec_at(vec_t* vec, size_t n) {
    assert(vec != NULL);

    if (n >= vec->__size) fail("index %zu out of range.\n", n);

    return vec->__start + vec->__el_size * n;
}

void* vec_front(vec_t* vec) {
    assert(vec != NULL);

    if (vec->__size == 0) return NULL;

    return vec->__start;
}

void* vec_back(vec_t* vec) {
    assert(vec != NULL);

    if (vec->__size == 0) return NULL;

    return vec->__start + vec->__el_size * (vec->__size - 1);
}

bool vec_is_empty(vec_t* vec) {
    assert(vec != NULL);
    return vec->__size == 0;
}

size_t vec_size(vec_t* vec) {
    assert(vec != NULL);
    return vec->__size;
}

size_t vec_max_size(vec_t* vec) {
    assert(vec != NULL);
    return SIZE_MAX / vec->__el_size;
}

size_t vec_capacity(vec_t* vec) {
    assert(vec != NULL);
    return vec->__capacity;
}

void vec_shrink_to_fit(vec_t* vec) {
    assert(vec != NULL);

    vec_unsafe_resize(vec, vec->__size);
}

void vec_clear(vec_t* vec) {
    assert(vec != NULL);

    vec_unsafe_resize(vec, 0);
    vec->__size = 0;
}

void vec_push_back(vec_t* vec, void* el) {
    vec_safe_check(vec);
    void* dest = (char*)vec->__start + vec->__el_size * vec->__size;

#ifdef SIMD_ENABLED
    if (vec->__el_size % 32 == 0) {
        __m256i* dest_avx = (__m256i*)dest;
        __m256i* src_avx = (__m256i*)el;

        for (size_t i = 0; i < vec->__el_size / 32; ++i) {
            _mm256_storeu_si256(&dest_avx[i], _mm256_loadu_si256(&src_avx[i]));
        }
    } else if (vec->__el_size % 16 == 0) {
        __m128i* dest_sse = (__m128i*)dest;
        __m128i* src_sse = (__m128i*)el;

        for (size_t i = 0; i < vec->__el_size / 16; ++i) {
            _mm_storeu_si128(&dest_sse[i], _mm_loadu_si128(&src_sse[i]));
        }
    } else {
        memcpy(dest, el, vec->__el_size);
    }
#else
    memcpy(dest, el, vec->__el_size);
#endif
    vec->__size++;
}

void vec_append(vec_t* dest, vec_t* from) {
    assert(dest->__el_size == from->__el_size);
    vec_reserve(dest, dest->__size + from->__size);

#ifndef SIMD_ENABLED
    memcpy((char*)dest->__start + dest->__el_size * dest->__size, from->__start, from->__el_size * from->__size);
#else
    char* dest_ptr = (char*)dest->__start + dest->__el_size * dest->__size;
    char* src_ptr = (char*)from->__start;

    size_t total_bytes = from->__el_size * from->__size;
    size_t i = 0;

    for (; i + 31 < total_bytes; i += 32) {
        __m256i data = _mm256_loadu_si256((__m256i*)(src_ptr + i));
        _mm256_storeu_si256((__m256i*)(dest_ptr + i), data);
    }

    if (i < total_bytes) {
        memcpy(dest_ptr + i, src_ptr + i, total_bytes - i);
    }
#endif
    dest->__size += from->__size;
}

void* vec_pop_back(vec_t* vec) {
    void* end = vec_back(vec);

    --vec->__size;

    return end;
}
