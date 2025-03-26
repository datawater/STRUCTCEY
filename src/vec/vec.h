#include "../utils.h"

#if defined(SMALL_VEC) || defined(E32BIT) == 4
#define VE_EL_SIZE_T u8
#else
#define VE_EL_SIZE_T u16
#endif  // SMALL_VEC

typedef struct vec_s {
    VE_EL_SIZE_T __el_size;  // in bytes
    size_t __capacity;
    size_t __size;

    char* __start;
} vec_t;

#define vec_new(type) __vec_new(sizeof(type))

vec_t __vec_new(VE_EL_SIZE_T element_size);
vec_t vec_from_array(VE_EL_SIZE_T element_size, size_t element_count, void* elements);

void* vec_at(vec_t* vec, size_t n);
void* vec_front(vec_t* vec);
void* vec_back(vec_t* vec);

bool vec_is_empty(vec_t* vec);
size_t vec_size(vec_t* vec);
size_t vec_max_size(vec_t* vec);
size_t vec_capacity(vec_t* vec);
void vec_reserve(vec_t* vec, size_t new_capacity);
void vec_shrink_to_fit(vec_t* vec);

void vec_clear(vec_t* vec);
void vec_push_back(vec_t* vec, void* el);
void vec_append(vec_t* dest, vec_t* from);
void* vec_pop_back(vec_t* vec);
