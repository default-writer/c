#include "std/list.h"
#include "list-experimental/data.h"
#include "common/alloc.h"

const int _allocation_size = 8*sizeof(void*);

struct list_data* _new() {
    /* external code allocates memory and resets memort block to zero  */
    struct list_data* ptr = _list_alloc(1, size());
    ptr->size = _allocation_size;
    ptr->data = _list_alloc(1, _allocation_size);
    ptr->data[0] = (void*)(ptr->data);
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

/* gets chunk's next item. external code enshures ptr is not 0 */
struct list_data* list_next(struct list_data *ptr) {
    /* external code enshures prt is not 0 */
    return ptr->next;
}

/* allocates a memory for provided payload  */
/* at current context, data payload stored at allocated memory buffer */
/* as a result, items counter will increase */
void list_push(struct list_data** const current, const void* payload) {
    const struct list_data * tmp = *current;
    if (tmp != 0) {
        struct list_data* ptr = *current;
        /* increase starting address */
        ptr->data[0] += sizeof(void*);
        LPTR offset = (ptr->data[0] - (void*)(ptr->data));
        if (offset >= ptr->size) {
            ptr->size += _allocation_size;
            ptr->data = _list_realloc(ptr->data, ptr->size);
            ptr->data[0] = (void*)(ptr->data) + offset;
        }
        const void **data = (void*)(ptr->data) + offset;
        *data = payload;
    }
}

/* pop existing element at the top of the stack/queue/list */
const void* list_pop(struct list_data** const current) {
    const struct list_data* tmp = *current;
    if (tmp != 0) {    
        /* get current context's head */
        struct list_data* ptr = *current;
        /* if we call method on empty stack, do not return head element, return null element by convention */
        if (ptr && ptr->data[0] != ptr->data) {
            /* gets temporary pointer value */
            /* returns actual data */
            LPTR offset = (ptr->data[0] - (void*)ptr->data);
            // gets data pointer
            void **data = (void*)(ptr->data) + offset;
            const void* payload = *data;
#ifdef USE_MEMORY_CLEANUP
            // cleaups memory
            *data = 0;
#endif
            /* free temporary pointer value */        
            ptr->data[0] -= sizeof(void*);
            /* returns removed element */
            return payload;
        }
        /* get current context's head */
        ptr = *current;
        /* if we call method on empty stack, do not return head element, return null element by convention */
        /* gets next pointer */
        struct list_data* next = list_next(ptr);
        /* if we call method on empty stack, do not return head element, return null element by convention */
        if (next == 0) {
            /* returns default element as null element */
            return 0;
        }
        /* rewinds head pointer to next pointer value */
        *current = next;
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

/* peek existing element at the top of the stack/queue/list */
/* at current context, existing head */
const void* list_peek(struct list_data** const current) {
    const struct list_data* tmp = *current;
    if (tmp != 0) {
        /* get current context's head */
        struct list_data* ptr = *current;
        /* if we call method on empty stack, do not return head element, return null element by convention */
        if (ptr && ptr->data[0] != ptr->data) {
            /* returns actual data */
            LPTR offset = (ptr->data[0] - (void*)ptr->data);
            // gets data pointer
            void **data = (void*)(ptr->data) + offset;
            const void* payload = *data;
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
