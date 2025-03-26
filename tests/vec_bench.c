#include "vec_bench.h"

#include <stdint.h>

#include "../src/vec/vec.h"

void vec_push_back_bench_int() {
    WORKSHY_BLACK_BOX({
        volatile vec_t t = vec_new(int);

        for (volatile int i = 0; i < UINT16_MAX; i++) {
            vec_push_back((vec_t*)&t, (int*)&i);
        }
    });
}

typedef struct {
    u64 a;
    u64 b;
    u64 c;
    u64 d;
} s_32bytes;

#define S32BYTES(a, b, c, d) ((s_32bytes){(a), (b), (c), (d)})

void vec_push_back_bench_32() {
    WORKSHY_BLACK_BOX({
        volatile vec_t t = vec_new(s_32bytes);

        for (volatile int i = 0; i < UINT16_MAX; i++) {
            volatile s_32bytes b = S32BYTES(i, i + 1, i + 2, i + 3);
            vec_push_back((vec_t*)&t, (s_32bytes*)&b);
        }
    });
}

void vec_append_bench() {
    WORKSHY_BLACK_BOX({
        volatile vec_t t1 = vec_new(int);

        for (volatile int i = 0; i < UINT16_MAX / 2; i++) {
            vec_push_back((vec_t*)&t1, (int*)&i);
        }

        volatile vec_t t2 = vec_new(int);

        for (volatile int i = 0; i < UINT16_MAX / 2; i++) {
            vec_push_back((vec_t*)&t1, (int*)&i);
        }

        vec_append((vec_t*)&t1, (vec_t*)&t2);
    });
}