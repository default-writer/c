/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:15:15 GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "common/memory.h"
#include "list-light/data.h"

/* private */

/* size of a memory block to allocate */
static const size_t _size = sizeof(struct list_data);

/* allocates memory pointer */
static struct list_data* _new(void) {
    /* returns list object */
    return memory->alloc(_size);
}

/* releases memory pointer */
static void _delete(struct list_data* ptr) {
    /* releases the pointer */
    memory->free(ptr, _size);
}

/* ptr is not 0 */
static struct list_data* list_next(struct list_data* ptr) {
    /* ptr is not 0 */
    return ptr->next;
}

/* ptr is not 0 */
static void* list_data(struct list_data* ptr) {
    /* ptr is not 0 */
    return ptr->data;
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
    /* gets the current memory pointer */
    struct list_data* head = *current;
    /* assigns item pointer to head's prev pointer value */
    head->prev = item;
    /* assigns item's next pointer to current pointer */
    item->next = *current;
    /* advances position of head pointer to the new head */
    *current = item;
    /* returns created data structure */
    return item;
}

/* pushes the memory pointer */
static void list_push(struct list_data** current, void* payload) {
    const struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* creates empty data chunk */
        struct list_data* item = list_new(current);
        /* writes data into allocated memory buffer */
        item->data = payload;
    }
}

/* pops existing element at the top of the stack/queue/list */
static void* list_pop(struct list_data** current) {
    const struct list_data* tmp = *current;
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
static void* list_peek(struct list_data** current) {
    const struct list_data* tmp = *current;
    /* checks if pointer is not null */
    if (tmp != 0) {
        /* gets the current memory pointer */
        struct list_data* ptr = *current;
        /* gets next pointer */
        const struct list_data* next = list_next(ptr);
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
static void list_init(struct list_data** current) {
    const struct list_data* tmp = *current;
    /* checks if pointer is null */
    if (tmp == 0) {
        /* sets the current memory pointer */
        *current = _new();
    }
}

/* destroys the memory stack */
static void list_destroy(struct list_data** current) {
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

#ifdef USE_VM_DEBUG_INFO

/* prints head on current context (stack) */
static void list_print_head(struct list_data** current) {
    /* get current context's head */
    struct list_data* ptr = *current;
    /* visualize item */
    printf("   *: %016llx > %016llx\n", (u64)ptr, (u64)list_data(ptr));
}

/* prints all stack trace to output */
static void list_print(struct list_data** current) {
    /* sets the counter */
    int i = 0;
    /* assigns current's head pointer to the temporary */
    struct list_data* tmp = *current;
    if (tmp != 0) {
        /* until we found root element (element with no previous element reference) */
        do {
            /* debug output of memory dump */
            printf("%4d: %016llx * %016llx\n", ++i, (u64)tmp, (u64)list_data(tmp));
            /* remember temporary's prior pointer value to temporary */
            tmp = list_next(tmp);
        } while (tmp != 0 /*root*/);
    }
    /* stop on root element */
}

#endif

/* public */

const struct list list_light_definition = {
    /* generic methods */
    .init = list_init,
    .destroy = list_destroy,
    /* list methods */
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
#ifdef USE_VM_DEBUG_INFO
    .print_head = list_print_head,
    .print = list_print
#endif
};
