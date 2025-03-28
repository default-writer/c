/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 22, 2025 at 7:32:05 AM GMT+3
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

#include "list_v1.h"

#include "system/api/api_v1.h"
#include "system/memory/memory_v1.h"

#define STACK_ELEMENT_TYPE_SIZE sizeof(stack_element_type)

/* private */

static stack_ptr list_next(stack_ptr ptr);
static void* list_data(stack_ptr ptr);
static void list_delete(stack_ptr ptr);
static void list_push(stack_ptr* current, void* payload);
static void* list_pop(stack_ptr* current);
static void* list_peek(stack_ptr* current);
static void list_init(stack_ptr* current);
static void list_destroy(stack_ptr* current);
#ifdef USE_MEMORY_DEBUG_INFO
static void list_print_head(stack_ptr* current);
static void list_print(stack_ptr* current);
#endif

/* ptr is not 0 */
static stack_ptr list_next(stack_ptr ptr) {
    /* ptr is not 0 */
    return ptr->next;
}

/* ptr is not 0 */
static void* list_data(stack_ptr ptr) {
    /* ptr is not 0 */
    return ptr->data;
}

/* deletes the data pointer */
static void list_delete(stack_ptr ptr) {
    /* releases the pointer */
    api->free(ptr);
}

/* pushes the memory pointer */
static void list_push(stack_ptr* current, void* payload) {
    if (current == 0 || *current == 0) {
        return;
    }
    /* creates empty data chunk */
    stack_ptr item = api->alloc(1, STACK_ELEMENT_TYPE_SIZE);
    /* writes data into allocated memory buffer */
    item->data = payload;
    /* assigns item's next pointer to current pointer */
    item->next = *current;
    /* advances position of head pointer to the new head */
    *current = item;
}

/* pop existing element at the top of the stack/queue/list */
static void* list_pop(stack_ptr* current) {
    if (current == 0 || *current == 0) {
        return NULL_PTR;
    }
    /* gets the current memory pointer */
    stack_ptr ptr = *current;
    /* gets next pointer */
    stack_ptr next = list_next(ptr);
    /* if we call method on empty stack, do not return head element, return NULL_PTR element by convention */
    if (next == 0) {
        /* returns default element as NULL_PTR element */
        return NULL_PTR;
    }
    /* rewinds head pointer to next pointer value */
    *current = next;
    /* gets temporary pointer value */
    void* payload = list_data(ptr);
    /* releases memory */
    list_delete(ptr);
    /* returns removed element */
    return payload;
}

/* peeks existing element at the top of the stack/queue/list */
static void* list_peek(stack_ptr* current) {
    if (current == 0 || *current == 0) {
        return NULL_PTR;
    }
    /* gets the current memory pointer */
    stack_ptr ptr = *current;
    /* gets next pointer */
    const_stack_element_ptr next = list_next(ptr);
    /* if we call method on empty stack, do not return head element, return NULL_PTR element by convention */
    if (next == 0) {
        /* returns default element as NULL_PTR element */
        return NULL_PTR;
    }
    /* returns actual data */
    return list_data(ptr);
}

/* initializes the new context's head element */
static void list_init(stack_ptr* current) {
    /* checks if pointer is NULL_PTR */
    if (current == 0 || *current != 0) {
        return;
    }
    /* sets the current item */
    *current = api->alloc(1, STACK_ELEMENT_TYPE_SIZE);
}

/* destroys the memory stack */
static void list_destroy(stack_ptr* current) {
    if (current == 0 || *current == 0) {
        return;
    }
    /* gets the current memory pointer */
    stack_ptr tmp = *current;
    /* until we found element with no next node (not a list element) */
    do {
        /* gets temporary pointer value */
        stack_ptr ptr = tmp;
        /* gets prev pointer value */
        stack_ptr next = list_next(ptr);
        /* releases memory, should check for 0 before execution */
        list_delete(ptr);
        /* advances temporary pointer value to the next item */
        tmp = next;
    } while (tmp != 0);
    /* resets current pointer to 0 */
    *current = 0;
}

#ifdef USE_MEMORY_DEBUG_INFO

/* prints head on current context (stack) */
static void list_print_head(stack_ptr* current) {
    /* get current context's head */
    stack_ptr ptr = *current;
    /* visualize item */
    printf("  l.: %016llx > %016llx\n", (u64)ptr, (u64)list_data(ptr));
}

/* prints all stack trace to output */
static void list_print(stack_ptr* current) {
    /* sets the counter */
    u64 address = 0;
    /* assigns current's head pointer to the temporary */
    stack_ptr tmp = *current;
    if (tmp != 0) {
        /* until we found root element (element with no previous element reference) */
        do {
            /* debug output of memory dump */
            printf("  l.: %016llx ! %016llx > %016llx : %016llx\n", address, (u64)tmp, (u64)list_data(tmp), (u64)tmp->next);
            address++;
            /* remember temporary's prior pointer value to temporary */
            tmp = list_next(tmp);
        } while (tmp != 0 /*root*/);
    }
    /* stop on root element */
}

#endif

/* public */

const system_list_methods PRIVATE_API(system_list_methods_definitions) = {
    /* generic methods */
    .init = list_init,
    .destroy = list_destroy,
    /* list methods */
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
#ifdef USE_MEMORY_DEBUG_INFO
    .print_head = list_print_head,
    .print = list_print
#endif
};

const system_list_methods* CALL(system_list) {
    return &PRIVATE_API(system_list_methods_definitions);
}
