#include "std/list.h"
#include "list-alloc/data.h"
#include "common/alloc.h"

/* item size */
const int _item_size = sizeof(void*);
/* buffer size in bytes = size of 8 items */
const int _allocation_size = 8*_item_size;

struct list_data* _new() {
    /* external code allocates memory and resets memory block to zero  */
    struct list_data* ptr = _list_alloc(1, size());
    ptr->data = _list_alloc(1, _allocation_size);
    ptr->data[0] = (void*)(ptr->data);    
    ptr->size = _allocation_size;
    return ptr;
}

void _delete(struct list_data* ptr) {
    _list_free(ptr->data, ptr->size);
    _list_free(ptr, size());
}

/* gets size of a memory block to allocate */
size_t size() {
    /* returns size of a memory block to allocate */
    return sizeof(struct list_data);
}

/* allocates a memory for provided payload  */
/* at current context, data payload stored at allocated memory buffer */
/* as a result, items counter will increase */
void list_push(struct list_data** const current, const void* payload) {
    const struct list_data * tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        struct list_data* ptr = *current;
        /* increase starting address */
        LPTR offset = ptr->data[0] - (void*)(ptr->data);
        if (offset == ptr->size - _item_size) {
            ptr->data = _list_realloc(ptr->data, ptr->size + _allocation_size);
            ptr->data[0] = (void*)(ptr->data);
            ptr->size += _allocation_size;
        }
        ptr->data[0] += _item_size;
        // gets data pointer
        const void **data = ptr->data[0];
        *data = payload;
    }
}

/* pop existing element at the top of the stack/queue/list */
const void* list_pop(struct list_data** const current) {
    const struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        /* checks if pointer is not null or data exists */
        if (ptr && ptr->data[0] != ptr->data) {
            /* gets temporary pointer value */
            /* returns actual data */
            // gets data pointer
            const void **data = ptr->data[0];
            // gets the payload
            const void* payload = *data;
#ifdef USE_MEMORY_CLEANUP
            // resets the memory pointer
            *data = 0;
#endif
            /* free temporary pointer value */        
            ptr->data[0] -= _item_size;
            /* returns removed element */
            return payload;
        }
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

/* peek existing element at the top of the stack/queue/list */
/* at current context, existing head */
const void* list_peek(struct list_data** const current) {
    const struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        const struct list_data* ptr = *current;
        /* if we call method on empty stack, do not return head element, return null element by convention */
        if (ptr && ptr->data[0] != ptr->data) {
            // gets data pointer
            const void **data = ptr->data[0];
            // gets the payload
            const void* payload = *data;
            // returns payload
            return payload;
        }
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

const struct list list_alloc_definition = {
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};
