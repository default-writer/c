#include "std/list.h"
#include "list-experimental/data.h"
#include "common/alloc.h"

/* buffer size in bytes = size of n items */
const int _allocation_size = (N + 1)*sizeof(void*);

inline struct list_data* _new() {
    /* external code allocates memory and resets memory block to zero  */
    struct list_data* ptr = _list_alloc(1, size());
    ptr->data = _list_alloc(1, _allocation_size);
    ptr->data[0] = ptr->data;
    return ptr;
}

inline void _delete(struct list_data* ptr) {
    _list_free(ptr->data, _allocation_size);
    _list_free(ptr, size());
}

/* gets size of a memory block to allocate */
inline size_t size() {
    /* returns size of a memory block to allocate */
    return sizeof(struct list_data);
}

/* gets chunk's next item. external code enshures ptr is not 0 */
inline struct list_data* list_next(struct list_data* ptr) {
    /* external code enshures prt is not 0 */
    return ptr->next;
}

/* allocates a memory for provided payload  */
/* at current context, data payload stored at allocated memory buffer */
/* as a result, items counter will increase */
void list_push(struct list_data** const current, const void* payload) {
    const struct list_data*  tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        // gets data pointer
        const void** data = ptr->data[0];
        /* gets the current data offset for new data allocation */
        LPTR offset = (void*)(data + 1) - (void*)(ptr->data);
        /* checks if current data pointer allocated all data */
        if (offset == _allocation_size) {
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
const void* list_pop(struct list_data** const current) {
    const struct list_data* tmp = *current;
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
            _delete(ptr);
            /* updates pointer */
            ptr = next;
        }
        if (ptr && ptr->data[0] != ptr->data) {
            // gets data pointer
            void** data = ptr->data[0];
            // gets the payload
            const void* payload = *data;
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
const void* list_peek(struct list_data** const current) {
    const struct list_data* tmp = *current;
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
            // gets data pointer
            const void** data = ptr->data[0];
            // gets the payload
            const void* payload = *data;
            // returns payload
            return payload;
        }
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

const struct list list_experimental_definition = {
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};
