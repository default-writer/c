/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include "common/alloc.h"
#include "common/parameters.h"
#include "list-alloc/data.h"

/* macros */
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define ALLOC_SIZE(n) (((n) + 1) * PTR_SIZE) /* buffer size in bytes = size of 8 items */

/* list parameters definition */
extern struct list_parameters list_parameters_definition;

/* implementation */

/* definition */
extern const struct memory memory_definition;
static const struct memory* memory = &memory_definition;

/* private */

/* size of a memory block to allocate */
static const size_t _size = sizeof(struct list_data);

/* allocates memory pointer */
static struct list_data* _new(void) {
    /* declares pointer to list parameters definitions */
    const struct list_parameters* parameters = &list_parameters_definition;
    /* allocates memory */
    struct list_data* ptr = memory->alloc(_size);
    /* allocates nested memory pointer */
    ptr->data = memory->alloc(ALLOC_SIZE(parameters->block_size));
    /* sets the head */
    ptr->data[0] = ptr->data;
    /* sets the size */
    ptr->size = ALLOC_SIZE(parameters->block_size);
    /* returns list object */
    return ptr;
}

/* releases memory pointer */
static void _delete(struct list_data* ptr) {
    /* releases the nested memory pointer */
    memory->free(ptr->data, ptr->size);
    /* releases the pointer */
    memory->free(ptr, _size);
}

/* ptr is not 0 */
static void* list_data(struct list_data* ptr) {
    /* gets data pointer */
    void** data = ptr->data[0];
    /* gets the payload */
    void* payload = *data;
    /* returns payload */
    return payload;
}

/* pushes the memory pointer */
static void list_push(struct list_data** current, void* payload) {
    /* declares pointer to list parameters definitions */
    const struct list_parameters* parameters = &list_parameters_definition;
    const struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        /* gets data pointer */
        void** data = ptr->data[0];
        /* gets the current data offset for new data allocation */
        u64 offset = (u64)((u8*)(++data) - (u8*)(ptr->data));
        /* checks if current data pointer allocated all data */
        if (offset == ptr->size) {
            /* reallocates current data pointer to the new memory location */
            ptr->data = global_realloc(ptr->data, ptr->size, ptr->size + ALLOC_SIZE(parameters->block_size));
            /* updates the size of current data chunk */
            ptr->size += ALLOC_SIZE(parameters->block_size);
            /* updates current data pointer */
            data = (void**)(void*)((u8*)ptr->data + offset);
        }
        /* advances the current data pointer, writes data into allocated memory buffer */
        ptr->data[0] = data;
        /* writes down the current data pointer */
        *data = payload;
    }
}

/* pops existing element at the top of the stack/queue/list */
static void* list_pop(struct list_data** current) {
    const struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        /* checks if pointer is not null or data exists */
        if (ptr && ptr->data[0] != ptr->data) {
            /* gets data pointer */
            void** data = ptr->data[0];
            /* gets the payload */
            void* payload = *data;
#ifdef USE_MEMORY_CLEANUP
            /* resets the memory pointer, rewinds the current data pointer */
            *data = 0;
#endif
            /* rewinds the pointer */
            --data;
            /* writes down the current data pointer */
            ptr->data[0] = data;
            /* returns removed element */
            return payload;
        }
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

/* peeks existing element at the top of the stack/queue/list */
static void* list_peek(struct list_data** current) {
    const struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        /* if we call method on empty stack, do not return head element, return null element by convention */
        if (ptr && ptr->data[0] != ptr->data) {
            /* returns data */
            return list_data(ptr);
        }
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

#ifdef VM_DEBUG_INFO

/* prints head on current context (stack) */
static void list_print_head(struct list_data** current) {
    /* get current context's head */
    const struct list_data* ptr = *current;
    /* gets data pointer */
    void** data = ptr->data[0];
    /* prints data value */
    printf("   *: %016llx > %016llx\n", (u64)ptr->data[0], (u64)*data);
}

/* prints all stack trace to output */
static void list_print(struct list_data** current) {
    /* get current context's head */
    struct list_data* ptr = *current;
    /* sets the counter */
    int i = 0;
    /* assigns current's head pointer to the temporary */
    void* end = ptr->data[0];
    if (end != ptr->data) {
        /* until we found root element (element with no previous element reference) */
        do {
            ++i;
            /* debug output of memory dump */
            printf("%4d: %016llx * %016llx\n", i, (u64)end, (u64)ptr->data[i]);
            /* remember temporary's prior pointer value to temporary */
            end = ptr->data + i;
        } while (ptr->data[0] != end /*root*/);
    }
    /* stop on root element */
}

#endif

/* public */

/* initializes the new context's head element */
struct list_data* list_init(void) {
    return _new();
}

/* destroys the memory stack */
void list_destroy(struct list_data* ptr) {
    _delete(ptr);
}

const struct list list_alloc_definition = {
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
#ifdef VM_DEBUG_INFO
    .print_head = list_print_head,
    .print = list_print
#endif
};
