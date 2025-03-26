#pragma once

#include "../utils.h"

#if defined(SMALL_SLL) || defined(E32BIT) == 4
#define SLL_EL_SIZE_T u8
#else
#define SLL_EL_SIZE_T u16
#endif  // SMALL_SLL

typedef struct sll_node_s {
    void* data;
    struct sll_node_s* next;
} sll_node;

typedef struct sll_root_s {
    SLL_EL_SIZE_T __element_size;
    sll_node* __root;
    sll_node* __leaf;
    size_t __size;
} sll_root;

#define sll_new_root_from_node(node, type) __sll_root_from_node((node), sizeof((type)))
#define sll_new_node(data, type) __sll_new_node((data), sizeof((type)))

sll_root __sll_root_from_node(sll_node* node, SLL_EL_SIZE_T element_size);
sll_node* __sll_new_node(void* data, SLL_EL_SIZE_T data_size);
size_t sll_length(sll_root* r);
sll_node* sll_find_end(sll_root* r);
sll_node* sll_at(sll_root* r, size_t index);

void sll_push_front(sll_root* r, void* value);
void sll_push_back(sll_root* r, void* value);
void sll_push_front_node(sll_root* r, sll_node* i);
void sll_push_back_node (sll_root* r, sll_node* i);
sll_node* sll_pop_front(sll_root* r);
sll_node* sll_pop_back(sll_root* r);
void sll_insert_at(sll_root* r, sll_node* i, size_t index);
sll_node* sll_pop_at(sll_root* r, size_t index);
sll_root sll_cut_off_at(sll_root* r, size_t index);
void sll_destroy(sll_root* r);

void sll_traverse(sll_root* r, void(*lambda)(sll_node*));