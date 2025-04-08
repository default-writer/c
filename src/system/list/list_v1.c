/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 8, 2025 at 1:18:13 PM GMT+3
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

#include "system/hashtable/hashtable_v1.h"
#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#define STACK_ELEMENT_TYPE_SIZE sizeof(stack_element_type)

/* private */

static stack_ptr list_next(stack_ptr ptr);
static void_ptr list_data(stack_ptr ptr);
static void list_delete(stack_ptr ptr);
static void list_push(stack_ptr* current, void_ptr payload);
static void_ptr list_pop(stack_ptr* current);
static void_ptr list_peek(stack_ptr* current);
static void list_init(stack_ptr* current);
static void list_destroy(stack_ptr* current);
static void list_diff(stack_ptr* stack1, stack_ptr* stack2, stack_ptr* compare);
static void list_diff_left(stack_ptr* stack1, stack_ptr* stack2, stack_ptr* compare);
static void list_diff_right(stack_ptr* stack1, stack_ptr* stack2, stack_ptr* compare);
#ifdef USE_MEMORY_DEBUG_INFO
static void list_print_head(stack_ptr* current);
static void list_print(stack_ptr* current);
#endif

/* internal */
static void list_diff_internal(stack_ptr* stack1, stack_ptr* stack2, stack_ptr* compare1, stack_ptr* compare2);

/* ptr is not 0 */
static stack_ptr list_next(stack_ptr ptr) {
    /* ptr is not 0 */
    return ptr->next;
}

/* ptr is not 0 */
static void_ptr list_data(stack_ptr ptr) {
    /* ptr is not 0 */
    return ptr->data;
}

/* deletes the data pointer */
static void list_delete(stack_ptr ptr) {
    /* releases the pointer */
    CALL(os)->free(ptr);
}

/* pushes the memory pointer */
static void list_push(stack_ptr* current, void_ptr payload) {
    if (current == 0 || *current == 0) {
        return;
    }
    /* creates empty data chunk */
    stack_ptr item = CALL(os)->calloc(1, STACK_ELEMENT_TYPE_SIZE);
    /* writes data into allocated memory buffer */
    item->data = payload;
    /* assigns item's next pointer to current pointer */
    item->next = *current;
    /* advances position of head pointer to the new head */
    *current = item;
}

/* pop existing element at the top of the stack/queue/list */
static void_ptr list_pop(stack_ptr* current) {
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
    void_ptr payload = list_data(ptr);
    /* releases memory */
    list_delete(ptr);
    /* returns removed element */
    return payload;
}

/* peeks existing element at the top of the stack/queue/list */
static void_ptr list_peek(stack_ptr* current) {
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
    *current = CALL(os)->calloc(1, STACK_ELEMENT_TYPE_SIZE);
}

static void list_diff(stack_ptr* stack1, stack_ptr* stack2, stack_ptr* compare) {
    if (stack1 == 0 || *stack1 == 0 || stack2 == 0 || *stack2 == 0 || compare == 0 || *compare == 0) {
        return;
    }
    list_diff_internal(stack1, stack2, compare, compare);
}

static void list_diff_left(stack_ptr* stack1, stack_ptr* stack2, stack_ptr* compare) {
    if (stack1 == 0 || *stack1 == 0 || stack2 == 0 || *stack2 == 0 || compare == 0 || *compare == 0) {
        return;
    }
    list_diff_internal(stack1, stack2, compare, NULL_PTR);
}

static void list_diff_right(stack_ptr* stack1, stack_ptr* stack2, stack_ptr* compare) {
    if (stack1 == 0 || *stack1 == 0 || stack2 == 0 || *stack2 == 0 || compare == 0 || *compare == 0) {
        return;
    }
    list_diff_internal(stack1, stack2, NULL_PTR, compare);
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
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
    const char* start = "\x1b[34m";
    const char* end = "\x1b[0m";
    fprintf(stderr, "%s[  l. ]%s: %016llx > %016llx\n", start, end, (u64)ptr, (u64)list_data(ptr)); /* NOLINT */
#else
    fprintf(stderr, "  l. : %016llx > %016llx\n", (u64)ptr, (u64)list_data(ptr)); /* NOLINT */
#endif
#endif
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
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
            const char* start = "\x1b[34m";
            const char* end = "\x1b[0m";
            fprintf(stderr, "%s[  l. ]%s: %016llx ! %016llx > %016llx : %016llx\n", start, end, address, (u64)tmp, (u64)list_data(tmp), (u64)tmp->next); /* NOLINT */
#else
            fprintf(stderr, "  l. : %016llx ! %016llx > %016llx : %016llx\n", address, (u64)tmp, (u64)list_data(tmp), (u64)tmp->next); /* NOLINT */
#endif
#endif
            address++;
            /* remember temporary's prior pointer value to temporary */
            tmp = list_next(tmp);
        } while (tmp != 0 /*root*/);
    }
    /* stop on root element */
}

#endif

/* internal */
static void list_diff_internal(stack_ptr* stack1, stack_ptr* stack2, stack_ptr* compare1, stack_ptr* compare2) {
    u64 ptr1 = 0;
    u64 ptr2 = 0;
    u64 forward_ptr1 = 0;
    u64 forward_ptr2 = 0;
    hashtable_ptr ht1 = CALL(hashtable)->init();
    stack_ptr stack1_current = *stack1;
    while (stack1_current != NULL_PTR) {
        ptr1 = (u64)stack1_current->data;
        stack1_current = stack1_current->next;
        if (CALL(hashtable)->get(ht1, ptr1) == NULL_PTR) {
            CALL(hashtable)->insert(ht1, ptr1, (void_ptr)ptr1);
        }
    }
    stack1_current = *stack1;
    hashtable_ptr ht2 = CALL(hashtable)->init();
    stack_ptr stack2_current = *stack2;
    while (stack2_current != NULL_PTR) {
        ptr2 = (u64)stack2_current->data;
        stack2_current = stack2_current->next;
        if (CALL(hashtable)->get(ht2, ptr2) == NULL_PTR) {
            CALL(hashtable)->insert(ht2, ptr2, (void_ptr)ptr2);
        }
    }
    stack2_current = *stack2;
    if (compare1 != 0 && *compare1 != 0) {
        while (stack1_current->next != NULL_PTR) {
            ptr1 = (u64)stack1_current->data;
            stack1_current = stack1_current->next;
            if (CALL(hashtable)->get(ht2, ptr1) == NULL_PTR) {
                CALL(list)->push(compare1, (void_ptr)ptr1);
            }
        }
    }
    if (compare2 != 0 && *compare2 != 0) {
        while (stack2_current->next != NULL_PTR) {
            ptr2 = (u64)stack2_current->data;
            stack2_current = stack2_current->next;
            if (CALL(hashtable)->get(ht1, ptr2) == NULL_PTR) {
                CALL(list)->push(compare2, (void_ptr)ptr2);
            }
        }
    }
    CALL(hashtable)->destroy(ht1);
    CALL(hashtable)->destroy(ht2);
}

/* public */

const system_list_methods PRIVATE_API(system_list_methods_definitions) = {
    /* generic methods */
    .init = list_init,
    .destroy = list_destroy,
    /* list methods */
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
    .diff = list_diff,
    .diff_left = list_diff_left,
    .diff_right = list_diff_right,
#ifdef USE_MEMORY_DEBUG_INFO
    .print_head = list_print_head,
    .print = list_print
#endif
};

const system_list_methods* PRIVATE_API(list) = &PRIVATE_API(system_list_methods_definitions);
const system_list_methods* CALL(list) {
    return PRIVATE_API(list);
}
