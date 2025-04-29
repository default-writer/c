/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 29, 2025 at 2:26:46 PM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "list_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/hashtable/hashtable_v1.h"
#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#define STACK_TYPE_SIZE sizeof(stack_type)
#define STACK_ELEMENT_TYPE_SIZE sizeof(stack_element_type)

/* private */
static stack_ptr list_init(void);
static u64 list_push(stack_ptr stack, void_ptr data);
static void_ptr list_peek(stack_ptr stack);
static void_ptr list_pop(stack_ptr stack);
static u64 list_size(stack_ptr stack);
static u64 list_diff(stack_ptr stack1, stack_ptr stack2, stack_ptr compare);
static u64 list_diff_left(stack_ptr stack1, stack_ptr stack2, stack_ptr compare);
static u64 list_diff_right(stack_ptr stack1, stack_ptr stack2, stack_ptr compare);
static u64 list_destroy(stack_ptr stack);
#ifdef USE_MEMORY_DEBUG_INFO
static void list_print_head(stack_ptr stack);
static void list_print(stack_ptr stack);
#endif

/* internal */
static void list_diff_internal(stack_ptr stack1, stack_ptr stack2, stack_ptr compare1, stack_ptr compare2);

/* initializes the new context's head element */
static stack_ptr list_init(void) {
    /* sets the current item */
    return CALL(os)->calloc(1, STACK_TYPE_SIZE);
}

/* pushes the memory pointer */
static u64 list_push(stack_ptr stack, void_ptr data) {
    CHECK_ARG(stack, FALSE);
    CHECK_ARG(data, FALSE);
    /* creates empty data chunk */
    stack_element_ptr item = CALL(os)->calloc(1, STACK_ELEMENT_TYPE_SIZE);
    /* writes data into allocated memory buffer */
    item->data = data;
    /* assigns item's next pointer to current pointer */
    item->next = stack->current;
    /* advances position of head pointer to the new head */
    stack->current = item;
    /* increment stack size */
    ++stack->size;
    /* returns success */
    return TRUE;
}

/* pop existing element at the top of the stack/queue/list */
static void_ptr list_pop(stack_ptr stack) {
    CHECK_ARG(stack, NULL_PTR);
    /* gets the current memory pointer */
    stack_element_ptr ptr = stack->current;
    /* no data added */
    CHECK_VALUE(ptr, NULL_PTR);
    /* rewinds head pointer to next pointer value */
    stack->current = ptr->next;
    /* gets temporary pointer value */
    void_ptr data = ptr->data;
    /* releases memory */
    CALL(os)->free(ptr);
    /* decrements stack size */
    --stack->size;
    /* returns removed element */
    return data;
}

/* peeks existing element at the top of the stack/queue/list */
static void_ptr list_peek(stack_ptr stack) {
    CHECK_ARG(stack, NULL_PTR);
    /* gets the current memory pointer */
    stack_element_ptr ptr = stack->current;
    /* no data added */
    CHECK_VALUE(ptr, NULL_PTR);
    /* returns actual data */
    return ptr->data;
}

static u64 list_diff(stack_ptr stack1, stack_ptr stack2, stack_ptr compare) {
    CHECK_ARG(stack1, FALSE);
    CHECK_ARG(stack2, FALSE);
    CHECK_ARG(compare, FALSE);
    list_diff_internal(stack1, stack2, compare, compare);
    /* returns success */
    return TRUE;
}

static u64 list_diff_left(stack_ptr stack1, stack_ptr stack2, stack_ptr compare) {
    CHECK_ARG(stack1, FALSE);
    CHECK_ARG(stack2, FALSE);
    CHECK_ARG(compare, FALSE);
    list_diff_internal(stack1, stack2, compare, NULL_PTR);
    /* returns success */
    return TRUE;
}

static u64 list_diff_right(stack_ptr stack1, stack_ptr stack2, stack_ptr compare) {
    CHECK_ARG(stack1, FALSE);
    CHECK_ARG(stack2, FALSE);
    CHECK_ARG(compare, FALSE);
    list_diff_internal(stack1, stack2, NULL_PTR, compare);
    /* returns success */
    return TRUE;
}

/* destroys the memory stack */
static u64 list_destroy(stack_ptr stack) {
    CHECK_ARG(stack, FALSE);
    /* gets the current memory pointer */
    stack_element_ptr ptr = stack->current;
    /* no data added */
    if (ptr != 0) {
        /* until we found element with no next node (not a list element) */
        do {
            /* gets prev pointer value */
            stack_element_ptr next = ptr->next;
            /* releases memory, should check for 0 before execution */
            CALL(os)->free(ptr);
            /* advances temporary pointer value to the next item */
            ptr = next;
        } while (ptr != 0);
    }
    /* releases memory */
    CALL(os)->free(stack);
    /* returns success */
    return TRUE;
}

#ifdef USE_MEMORY_DEBUG_INFO

/* prints head on current context (stack) */
static void list_print_head(stack_ptr stack) {
    /* get current context's head */
    stack_element_ptr ptr = stack->current;
    /* visualize item */
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
    const char* start = "\x1b[34m";
    const char* end = "\x1b[0m";
    fprintf(stderr, "%s[  l. ]%s: %016llx > %016llx\n", start, end, (u64)ptr, (u64)ptr->data); /* NOLINT */
#else
    fprintf(stderr, "  l. : %016llx > %016llx\n", (u64)ptr, (u64)ptr->data); /* NOLINT */
#endif
#endif
}

/* prints all stack trace to output */
static void list_print(stack_ptr stack) {
    /* sets the counter */
    u64 address = 0;
    /* assigns current's head pointer to the temporary */
    stack_element_ptr tmp = stack->current;
    if (tmp != 0) {
        /* until we found root element (element with no previous element reference) */
        do {
            /* debug output of memory dump */
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
            const char* start = "\x1b[34m";
            const char* end = "\x1b[0m";
            fprintf(stderr, "%s[  l. ]%s: %016llx ! %016llx > %016llx : %016llx\n", start, end, address, (u64)tmp, (u64)tmp->data, (u64)tmp->next); /* NOLINT */
#else
            fprintf(stderr, "  l. : %016llx ! %016llx > %016llx : %016llx\n", address, (u64)tmp, (u64)tmp->data, (u64)tmp->next); /* NOLINT */
#endif
#endif
            address++;
            /* remember temporary's prior pointer value to temporary */
            tmp = tmp->next;
        } while (tmp != 0 /*root*/);
    }
    /* stop on root element */
}

#endif

/* internal */
static void list_diff_internal(stack_ptr stack1, stack_ptr stack2, stack_ptr compare1, stack_ptr compare2) {
    void_ptr ptr1 = 0;
    void_ptr ptr2 = 0;
    hashtable_ptr ht1 = CALL(hashtable)->init();
    stack_element_ptr stack1_current = stack1->current;
    while (stack1_current != NULL_PTR) {
        ptr1 = stack1_current->data;
        stack1_current = stack1_current->next;
        if (CALL(hashtable)->get(ht1, (u64)ptr1) == NULL_PTR) {
            CALL(hashtable)->insert(ht1, (u64)ptr1, ptr1);
        }
    }
    hashtable_ptr ht2 = CALL(hashtable)->init();
    stack_element_ptr stack2_current = stack2->current;
    while (stack2_current != NULL_PTR) {
        ptr2 = stack2_current->data;
        stack2_current = stack2_current->next;
        if (CALL(hashtable)->get(ht2, (u64)ptr2) == NULL_PTR) {
            CALL(hashtable)->insert(ht2, (u64)ptr2, ptr2);
        }
    }
    if (compare1 != 0) {
        stack1_current = stack1->current;
        while (stack1_current != NULL_PTR) {
            ptr1 = stack1_current->data;
            stack1_current = stack1_current->next;
            if (CALL(hashtable)->get(ht2, (u64)ptr1) == NULL_PTR) {
                list_push(compare1, ptr1);
            }
        }
    }
    if (compare2 != 0) {
        stack2_current = stack2->current;
        while (stack2_current != NULL_PTR) {
            ptr2 = stack2_current->data;
            stack2_current = stack2_current->next;
            if (CALL(hashtable)->get(ht1, (u64)ptr2) == NULL_PTR) {
                list_push(compare2, ptr2);
            }
        }
    }
    CALL(hashtable)->destroy(ht1);
    CALL(hashtable)->destroy(ht2);
}

/* public */

const virtual_list_methods PRIVATE_API(virtual_list_methods_definitions) = {
    .init = list_init,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
    .diff = list_diff,
    .diff_left = list_diff_left,
    .diff_right = list_diff_right,
    .destroy = list_destroy,
#ifdef USE_MEMORY_DEBUG_INFO
    .print_head = list_print_head,
    .print = list_print
#endif
};

const virtual_list_methods* PRIVATE_API(list) = &PRIVATE_API(virtual_list_methods_definitions);
const virtual_list_methods* CALL(list) {
    return PRIVATE_API(list);
}
