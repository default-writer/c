#include "common/alloc.h"
#include "list-light/data.h"
#include "std/common.h"

/* gets size of a memory block to allocate */
size_t _size() {
    /* returns size of a memory block to allocate */
    return sizeof(struct list_data);
}

/* allocates memory pointer */
struct list_data* _new() {
    // returns list object
    return _list_alloc(1, _size());
}

/* releases memory pointer */
void _delete(struct list_data* ptr) {
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
    /* external code ensures prt is not 0 */
    return ptr->data;
}

/* deletes the data pointer */
void list_delete(struct list_data* ptr) {
    /* releases the pointer */
    _delete(ptr);
}

/* pushes the memory pointer */
void list_push(struct list_data** current, void* payload) {
    struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* creates empty data chunk */
        struct list_data* item = _new();
        /* writes data into allocated memory buffer */
        item->data = payload;
        /* gets the current memory pointer */
        struct list_data* head = *current;
        /* assigns item pointer to head's prev pointer value */
        head->prev = item;
        /* assigns item's next pointer to current pointer */
        item->next = *current;
        /* advances position of head pointer to the new head */
        *current = item;
    }
}

/* pops existing element at the top of the stack/queue/list */
void* list_pop(struct list_data** current) {
    struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        /* gets next pointer */
        struct list_data* next = list_next(ptr);
        /* root elements returns null, i.e. 0 by convention */
        if (next == 0) {
            /* returns default element as null element */
            return 0;
        }
        /* resets prev pointer */
        next->prev = 0;
        /* rewinds head pointer to next pointer value */
        *current = next;
        /* gets temporary pointer value */
        void* payload = list_data(ptr);
        /* releases memory */
        list_delete(ptr);
        /* returns removed element */
        return payload;
    }
    /* if we call method on empty stack, do not return head element, return null
     * element by convention */
    return 0;
}

/* peeks existing element at the top of the stack/queue/list */
void* list_peek(struct list_data** current) {
    struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        /* gets next pointer */
        struct list_data* next = list_next(ptr);
        /* if we call method on empty stack, do not return head element, return null
         * element by convention */
        if (next == 0) {
            /* returns default element as null element */
            return 0;
        }
        /* returns actual data */
        return list_data(ptr);
    }
    /* if we call method on empty stack, do not return head element, return null
     * element by convention */
    return 0;
}

/* initializes the new context's head element */
void list_init(struct list_data** current, struct list_data* (*list_new)()) {
    struct list_data* tmp = *current;
    if (tmp == 0) {
        /* sets the current memory pointer */
        *current = list_new();
    }
}

/* destroys the memory stack */
void list_destroy(struct list_data** current,
    void (*_list_delete)(struct list_data*),
    struct list_data* (*_list_next)(struct list_data*)) {
    /* gets the current memory pointer */
    struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* until we found element with no next node (not a list element) */
        do {
            /* gets temporary pointer value */
            struct list_data* ptr = tmp;
            /* gets prev pointer value */
            struct list_data* next = _list_next(ptr);
            /* releases memory, should check for 0 before execution */
            _list_delete(ptr);
            /* advances temporary pointer value to the next item */
            tmp = next;
        } while (tmp != 0);
        /* resets current pointer to 0 */
        *current = 0;
    }
}

const struct list list_light_definition = {
    // generic methods
    .init = list_init,
    .destroy = list_destroy,
    // list methods
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek
};