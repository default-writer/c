/*
 *
 * Russian's IP Protection License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person with citizenship
 * and location in Russia including Crimea and all occupations obtaining a copy
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
 * For the rest of the world it is an order to pay royalties by agreement to the
 * author of the code base for ability to use any part of the project for any
 * purpouse including but not limited to the creative ideas or technologies are
 * being used in this owned intellectual property.
 *
 * It is strictly prohibited to use this code base or any part of it for any purpouse
 * including prohibiting or restricive purpouses against Russians for any EU citizens
 * or other person with USA citizenship, origin or background including work permit
 * or locations from selected territories or any territory or any other country except
 * Russia considered as breaking basic human rights, freedom of speesh or involved in
 * acts of terrorism in a territory owned, occupied or managed by another country.
 *
 */
#include "common/memory.h"
#include "common/parameters.h"
#include "list-experimental/data.h"

/* macros */
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define ALLOC_SIZE(n) (((n) + 1) * PTR_SIZE) /* buffer size in bytes = size of 8 items */

extern const struct list_parameters list_parameters_definition;

/* implementation */

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
    /* returns list object */
    return ptr;
}

/* releases memory pointer */
static void _delete(struct list_data* ptr) {
    /* declares pointer to list parameters definitions */
    const struct list_parameters* parameters = &list_parameters_definition;
    /* releases the nested memory pointer */
    memory->free(ptr->data, ALLOC_SIZE(parameters->block_size));
    /* releases the pointer */
    memory->free(ptr, _size);
}

/* ptr is not 0 */
static struct list_data* list_next(struct list_data* ptr) {
    /* ptr is not 0 */
    return ptr->next;
}

/* ptr is not 0 */
static void* list_data(const struct list_data* ptr) {
    /* gets data pointer */
    void** data = ptr->data[0];
    /* gets the payload */
    void* payload = *data;
    /* returns payload */
    return payload;
}

/* deletes the data pointer */
static void list_delete(struct list_data* ptr) {
    /* releases the pointer */
    _delete(ptr);
}

/* allocates memory pointer */
static struct list_data* list_new(struct list_data** current) {
    /* creates empty data chunk */
    struct list_data* item = _new();
    /* assigns item's next pointer to current pointer */
    item->next = *current;
    /* advances position of head pointer to the new head */
    *current = item;
    /* returns created data structure */
    return item;
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
        if (offset == ALLOC_SIZE(parameters->block_size)) {
            /* updates current pointer */
            ptr = list_new(current);
            /* updates current data pointer */
            data = ptr->data[0];
            /* moves the pointer to the new block */
            ++data;
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
        if (ptr && ptr->data[0] == ptr->data) {
            /* updates pointer */
            ptr = list_next(ptr);
        }
        /* if we call method on empty stack, do not return head element, return null element by convention */
        if (ptr && ptr->data[0] != ptr->data) {
            /* returns data */
            return list_data(ptr);
        }
    }
    /* if we call method on empty stack, do not return head element, return null element by convention */
    return 0;
}

#ifdef USE_VM_DEBUG_INFO

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
    const void* end = ptr->data[0];
    if (end != ptr->data) {
        do {
            /* gets data pointer */
            void** data = ptr->data[0];
            /* until we found root element (element with no previous element reference) */
            do {
                /* debug output of memory dump */
                printf("%4d: %016llx * %016llx\n", ++i, (u64)data, (u64)*data);
            } while (ptr->data != --data /*root*/);
            /* gets next data pointer */
            ptr = list_next(ptr);
        } while (ptr != 0);
    }
}

#endif

/* public */

/* initializes the new context's head element */
void list_experimental_init(struct list_data** current) {
    const struct list_data* tmp = *current;
    /* checks if pointer is null */
    if (tmp == 0) {
        /* sets the current memory pointer */
        *current = _new();
    }
}

/* destroys the memory stack */
void list_experimental_destroy(struct list_data** current) {
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
    .peek = list_peek,
#ifdef USE_VM_DEBUG_INFO
    .print_head = list_print_head,
    .print = list_print
#endif
};
