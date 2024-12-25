#pragma once

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#if INTPTR_MAX == INT32_MAX
#define E32BIT
#elif INTPTR_MAX == INT64_MAX
#define E64BIT
#else
#error "Environment not 32 or 64-bit."
#endif

#define CONCAT_1(x, y) x##y
#define CONCAT_2(x, y) CONCAT_1(x, y)
#define CONCAT_3(x, y) CONCAT_2(x, y)
#define CONCAT_4(x, y) CONCAT_3(x, y)
#define CONCAT_5(x, y) CONCAT_4(x, y)
#define CONCAT_6(x, y) CONCAT_5(x, y)
#define CONCAT_7(x, y) CONCAT_6(x, y)
#define CONCAT_8(x, y) CONCAT_7(x, y)
#define CONCAT_9(x, y) CONCAT_8(x, y)
#define CONCAT(x, y) CONCAT_9(x, y)

#define UNUSED(x) ((void)(x))

#define PRINTF_BINARY_PATTERN_INT8 "%c%c%c%c%c%c%c%c"
#define PRINTF_BYTE_TO_BINARY_INT8(i)                                                             \
    (((i) & 0x80ll) ? '1' : '0'), (((i) & 0x40ll) ? '1' : '0'), (((i) & 0x20ll) ? '1' : '0'),     \
        (((i) & 0x10ll) ? '1' : '0'), (((i) & 0x08ll) ? '1' : '0'), (((i) & 0x04ll) ? '1' : '0'), \
        (((i) & 0x02ll) ? '1' : '0'), (((i) & 0x01ll) ? '1' : '0')

#define PRINTF_BINARY_PATTERN_INT16 PRINTF_BINARY_PATTERN_INT8 PRINTF_BINARY_PATTERN_INT8
#define PRINTF_BYTE_TO_BINARY_INT16(i) PRINTF_BYTE_TO_BINARY_INT8((i) >> 8), PRINTF_BYTE_TO_BINARY_INT8(i)
#define PRINTF_BINARY_PATTERN_INT32 PRINTF_BINARY_PATTERN_INT16 PRINTF_BINARY_PATTERN_INT16
#define PRINTF_BYTE_TO_BINARY_INT32(i) PRINTF_BYTE_TO_BINARY_INT16((i) >> 16), PRINTF_BYTE_TO_BINARY_INT16(i)
#define PRINTF_BINARY_PATTERN_INT64 PRINTF_BINARY_PATTERN_INT32 PRINTF_BINARY_PATTERN_INT32
#define PRINTF_BYTE_TO_BINARY_INT64(i) PRINTF_BYTE_TO_BINARY_INT32((i) >> 32), PRINTF_BYTE_TO_BINARY_INT32(i)

#define __log_skeleton(name, args...)                  \
    do {                                               \
        char* time = (char*)malloc(sizeof(char) * 64); \
        assert(time != NULL);                          \
        memset(time, 0, sizeof(char) * 64);            \
        format_current_time(time);                     \
        printf("%s " name " ", time);                  \
        printf(args);                                  \
        free(time);                                    \
    } while (0)

#define log(args...) __log_skeleton("[LOG] ", args)
#define info(args...) __log_skeleton("[INFO]", args)
#define warn(args...) __log_skeleton("[WARN]", args)

#define debug(args...)                                             \
    do {                                                           \
        printf("%s %s:%d [DEBUG] ", __func__, __FILE__, __LINE__); \
        printf(args);                                              \
    } while (0)

#define todo(args...)                                              \
    do {                                                           \
        printf("%s %s:%d [DEBUG] ", __func__, __FILE__, __LINE__); \
        printf(args);                                              \
        fflush(stdout);                                            \
        abort();                                                   \
    } while (0)

#define fail(args...)                    \
    do {                                 \
        __log_skeleton("[ERROR]", args); \
        abort();                         \
    } while (0)

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

void format_current_time(char* output);

static inline void* safe_malloc(size_t n) {
    void* x = malloc(n);

    if (x == NULL) fail("couldn't allocate memory\n");

    return x;
}

static inline void* safe_realloc(void* ptr, size_t n) {
    assert(ptr != NULL);

    void* x = realloc(ptr, n);

    if (x == NULL) {
        if (n > 0) fail("couldn't allocate memory\n");

        x = ptr;
    }

    return x;
}
