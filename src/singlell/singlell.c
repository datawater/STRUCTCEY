#include <assert.h>
#include <string.h>

#include "singlell.h"

static inline size_t sll_node_size(sll_node* node);

sll_root __sll_root_from_node(sll_node* node, SLL_EL_SIZE_T element_size) {
    assert(node != NULL);
    assert(element_size != 0);

    return (sll_root){.__element_size = element_size, .__root = node, .__leaf = node, .__size = sll_node_size(node)};
}

static inline size_t sll_node_size(sll_node* node) {
    size_t size = 0;

    while (node != NULL) {
        size++;
        node = node->next;
    }

    return size;
}

size_t sll_length(sll_root* r) { return sll_node_size(r->__root); }

sll_node* __sll_new_node(void* data, SLL_EL_SIZE_T data_size) {
    assert(data != NULL);
    assert(data_size != 0);

    sll_node* node = (sll_node*)safe_malloc(sizeof(sll_node));
    memcpy(node->data, data, data_size);

    return node;
}

sll_node* sll_find_end(sll_root* r) {
    assert(r != NULL);

    return r->__leaf;
}

sll_node* sll_at(sll_root* r, size_t index) {
    assert(r != NULL);

    if (index >= r->__size) return NULL;
    if (index == 0) return r->__root;
    if (index == r->__size - 1) return r->__leaf; 

    sll_node* n = r->__root;
    for (size_t i = 0; i < index; i++) {
        if (n == NULL) return n;

        n = n->next;
    }

    return n;
}

void sll_push_front(sll_root* r, void* value) {
    sll_push_front_node(r, __sll_new_node(value, r->__element_size));
}

void sll_push_back(sll_root* r, void* value) {
    sll_push_back_node(r, __sll_new_node(value, r->__element_size));
}

void sll_push_front_node(sll_root* r, sll_node* i) {
    assert(r != NULL);
    assert(i != NULL);

    i->next = r->__root;
    r->__root = i;
    r->__size++;
}
void sll_push_back_node(sll_root* r, sll_node* i) {
    assert(r != NULL);
    assert(i != NULL);
    
    r->__leaf->next = i;
    r->__leaf = r->__leaf->next;
    r->__size++;
}

sll_node* sll_pop_front(sll_root* r) {
    assert(r != NULL);

    sll_node* n = r->__root;

    r->__root = r->__root->next;
    r->__size--;

    return n;
}

sll_node* sll_pop_back(sll_root* r) {
    assert(r != NULL);

    sll_node* before_last = sll_at(r, r->__size - 2);
    sll_node* last = r->__leaf;

    before_last->next = NULL;
    r->__leaf = before_last;
    r->__size--;

    return last;
}

void sll_insert_at(sll_root* r, sll_node* i, size_t index) {
    assert(r != NULL);
    assert(index <= r->__size);

    if (index == 0) {
        sll_push_front_node(r, i);
        return;           
    }

    if (index == r->__size) {
        sll_push_back_node(r, i);
        return;
    }

    sll_node* before = sll_at(r, index - 1);
    sll_node* at = before->next;

    before->next = i;
    i->next = at;

    r->__size++;
}

sll_node* sll_pop_at(sll_root* r, size_t index) {
    assert(r != NULL);
    assert(index < r->__size);
    
    sll_node* before_at = sll_at(r, index - 1);
    sll_node* at = before_at->next;

    before_at->next = NULL;

    r->__size--;

    return at;
}

sll_root sll_cut_of_at(sll_root* r, size_t index) {
    assert(r != NULL);
    assert(index < r->__size);

    sll_node* before_at = sll_at(r, index - 1);
    sll_node* at = before_at->next;
    
    before_at->next = NULL;
    r->__size -= index + 1;

    return sll_new_root_from_node(at, r->__element_size);
}

void sll_destroy(sll_root* r) {
    assert(r != NULL);

    sll_node* current = r->__root;

    while (current != NULL) {
        sll_node* t = current;
        current = current->next;

        free(t);
    }
}

void sll_traverse(sll_root* r, void(*lambda)(sll_node*)) {
    assert(r != NULL);
    assert(lambda != NULL);

    sll_node* current = r->__root;

    while (current != NULL) {
        sll_node* t = current;
        current = current->next;

        lambda(t);
    }
}