#include "common/alloc.h"
#include "common/parameters.h"
#include "list-experimental/data.h"
#include "std/common.h"

/* buffer size in bytes = size of 8 items */
#define ALLOC_SIZE(n) ((n) + 1) * sizeof(void*)

extern const struct list_parameters list_parameters_definition;

/* gets size of a memory block to allocate */
size_t _size() {
    /* returns size of a memory block to allocate */
    return sizeof(struct list_data);
}

/* allocates memory pointer */
struct list_data* _new() {
    // declares pointer to list parameters definitions
    const struct list_parameters* parameters = &list_parameters_definition;
    /* allocates memory */
    struct list_data* ptr = _list_alloc(1, _size());
    /* allocates nested memory pointer */
    ptr->data = _list_alloc(1, ALLOC_SIZE(parameters->block_size));
    // sets the head
    ptr->data[0] = ptr->data;
    // returns list object
    return ptr;
}

/* releases memory pointer */
void _delete(struct list_data* ptr) {
    // declares pointer to list parameters definitions
    const struct list_parameters* parameters = &list_parameters_definition;
    /* releases the nested memory pointer */
    _list_free(ptr->data, ALLOC_SIZE(parameters->block_size));
    /* releases the pointer */
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
    /* releases the pointer */
    _delete(ptr);
}

/* pushes the memory pointer */
void list_push(struct list_data** current, void* payload) {
    // declares pointer to list parameters definitions
    const struct list_parameters* parameters = &list_parameters_definition;
    struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        // gets data pointer
        void** data = ptr->data[0];
        /* gets the current data offset for new data allocation */
        u64 offset = (u64)((u8*)(++data) - (u8*)(ptr->data));
        /* checks if current data pointer allocated all data */
        if (offset == ALLOC_SIZE(parameters->block_size)) {
            /* creates empty data chunk */
            struct list_data* item = _new();
            /* assigns item's next pointer to current pointer */
            item->next = *current;
            /* advances position of head pointer to the new head */
            *current = item;
            /* updates current pointer */
            ptr = *current;
            /* updates current data pointer */
            data = (void**)(item->data[0]) + 1;
        }
        // advances the current data pointer, writes data into allocated memory buffer */
        ptr->data[0] = data;
        // writes down the current data pointer
        *data = payload;
    }
}

/* pops existing element at the top of the stack/queue/list */
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
            /* releases memory */
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
            *data = 0;
#endif
            // rewinds the pointer
            --data;
            // writes down the current data pointer
            ptr->data[0] = data;
            /* returns removed element */
            return payload;
        }
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

/* peeks existing element at the top of the stack/queue/list */
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

/* initializes the new context's head element */
void list_init(struct list_data** current) {
    struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp == 0) {
        /* sets the current memory pointer */
        *current = _new();
    }
}

/* destroys the memory stack */
void list_destroy(struct list_data** current) {
    /* gets the current memory pointer */
    struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* until we found element with no next node (not a list element) */
        do {
            /* gets temporary pointer value */
            struct list_data* ptr = tmp;
            /* gets prev pointer value */
            struct list_data* next = list_next(ptr);
            /* releases memory, should check for 0 before execution */
            list_delete(ptr);
            /* advances temporary pointer value to the next item */
            tmp = next;
        } while (tmp != 0);
        /* resets current pointer to 0 */
        *current = 0;
    }
}

const struct list list_experimental_definition = {
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};
