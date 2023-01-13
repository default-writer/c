#include "common/alloc.h"
#include "list-experimental/data.h"
#include "std/list.h"

/* buffer size in bytes = size of n items */
#define ALLOC_SIZE (size_t)((N_ELEMENTS + 1) * sizeof(void*))

/* gets size of a memory block to allocate */
size_t _size() {
    /* returns size of a memory block to allocate */
    return sizeof(struct list_data);
}

struct list_data* _new() {
    /* external code allocates memory and resets memory block to zero  */
    struct list_data* ptr = _list_alloc(1, _size());
    ptr->data = _list_alloc(1, ALLOC_SIZE);
    ptr->data[0] = ptr->data;
    return ptr;
}

void _delete(struct list_data* ptr) {
    _list_free(ptr->data, ALLOC_SIZE);
    _list_free(ptr, _size());
}

/* gets chunk's next item. external code ensures ptr is not 0 */
struct list_data* list_next(struct list_data* ptr) {
    /* external code ensures prt is not 0 */
    return ptr->next;
}

/* gets chunk's payload. external code ensures ptr is not 0 */
void* list_data(struct list_data* ptr) {
    // gets data pointer
    void** data = ptr->data[0];
    // gets the payload
    void* payload = *data;
    // returns payload
    return payload;
}

/* deletes the data pointer */
void list_delete(struct list_data* ptr) {
    _delete(ptr);
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
        if (offset == ALLOC_SIZE) {
            /* creates empty data chunk */
            struct list_data* item = _new();
            /* assigns item's next pointer to current pointer */
            item->next = *current;
            /* advances position of head pointer to the new head */
            *current = item;
            /* updates current pointer */
            ptr = *current;
            /* updates current data pointer */
            data = item->data[0];
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
        /* if we call method on empty stack, do not return head element, return null element by convention */
        if (ptr && ptr->data[0] == ptr->data) {
            /* gets next pointer */
            struct list_data* next = list_next(ptr);
            /* if we call method on empty stack, do not return head element, return null element by convention */
            if (next == 0) {
                /* returns default element as null element */
                return 0;
            }
            /* rewinds head pointer to next pointer value */
            *current = next;
            /* frees up the memory */
            list_delete(ptr);
            /* updates pointer */
            ptr = next;
        }
        if (ptr && ptr->data[0] != ptr->data) {
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
        if (ptr && ptr->data[0] == ptr->data) {
            /* updates pointer */
            ptr = list_next(ptr);
        }
        /* if we call method on empty stack, do not return head element, return null element by convention */
        if (ptr && ptr->data[0] != ptr->data) {
            // returns data
            return list_data(ptr);
        }
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

struct list list_experimental_definition = {
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};
