#include "common/alloc.h"
#include "list-alloc/data.h"
#include "std/list.h"

/* buffer size in bytes = size of 8 items */
size_t _allocation_size = 8 * sizeof(void*);

/* gets size of a memory block to allocate */
size_t _size() {
    /* returns size of a memory block to allocate */
    return sizeof(struct list_data);
}

struct list_data* _new() {
    /* external code allocates memory and resets memory block to zero  */
    struct list_data* ptr = _list_alloc(1, _size());
    ptr->data = _list_alloc(1, _allocation_size);
    ptr->data[0] = ptr->data;
    ptr->size = _allocation_size;
    return ptr;
}

void _delete(struct list_data* ptr) {
    _list_free(ptr->data, ptr->size);
    _list_free(ptr, _size());
}

/* allocates a memory for provided payload  */
/* at current context, data payload stored at allocated memory buffer */
/* as a result, items counter will increase */
void list_push(struct list_data** current, void* payload) {
    struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        // gets data pointer
        void** data = ptr->data[0];
        /* gets the current data offset for new data allocation */
        __u_int64_t offset = (__u_int64_t)((__u_int8_t*)(data + 1) - (__u_int8_t*)(ptr->data));
        /* checks if current data pointer allocated all data */
        if (offset == ptr->size) {
            /* reallocates current data pointer to the new memory location */
            ptr->data = _list_realloc(ptr->data, ptr->size + _allocation_size);
            // writes down the current data pointer
            ptr->data[0] = ptr->data;
            // updates the size of current data chunk
            ptr->size += _allocation_size;
            /* updates current data pointer */
            data = ptr->data[0];
        }
        // advances the current data pointer, writes data into allocated memory buffer */
        *++data = payload;
        // writes down the current data pointer
        ptr->data[0] = data;
    }
}

/* pop existing element at the top of the stack/queue/list */
void* list_pop(struct list_data** current) {
    struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        /* checks if pointer is not null or data exists */
        if (ptr && ptr->data[0] != ptr->data) {
            /* gets temporary pointer value */
            /* returns actual data */
            // gets data pointer
            void** data = ptr->data[0];
            // gets the payload
            void* payload = *data;
#ifdef USE_MEMORY_CLEANUP
            // resets the memory pointer, rewinds the current data pointer
            *data-- = 0;
#endif
            // writes down the current data pointer
            ptr->data[0] = data;
            /* returns removed element */
            return payload;
        }
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

/* peek existing element at the top of the stack/queue/list */
/* at current context, existing head */
void* list_peek(struct list_data** current) {
    struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        /* if we call method on empty stack, do not return head element, return null element by convention */
        if (ptr && ptr->data[0] != ptr->data) {
            // gets data pointer
            void** data = ptr->data[0];
            // gets the payload
            void* payload = *data;
            // returns payload
            return payload;
        }
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

struct list list_alloc_definition = {
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};
