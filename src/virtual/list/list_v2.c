/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 29, 2025 at 6:06:53 PM GMT+3
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

#include "list_v2.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/hashtable/hashtable_v1.h"
#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

/* private */
static stack_v2_ptr list_init(u64 size, stack_v2_ptr stack);
static u64 list_push(stack_v2_ptr* stack_ptr, void_ptr data);
static void_ptr list_peek(stack_v2_ptr* stack);
static void_ptr list_pop(stack_v2_ptr* stack_ptr);
static u64 list_size(stack_v2_ptr stack);
static u64 list_diff(stack_v2_ptr stack1, stack_v2_ptr stack2, stack_v2_ptr* compare);
static u64 list_diff_left(stack_v2_ptr stack1, stack_v2_ptr stack2, stack_v2_ptr* compare);
static u64 list_diff_right(stack_v2_ptr stack1, stack_v2_ptr stack2, stack_v2_ptr* compare);
static u64 list_destroy(stack_v2_ptr* stack);

/* internal */
static void list_diff_internal(stack_v2_ptr stack1, stack_v2_ptr stack2, stack_v2_ptr* compare1, stack_v2_ptr* compare2);

/* initializes the new context's head element */
static stack_v2_ptr list_init(u64 size, stack_v2_ptr next) {
    u64 default_size = size == 0 ? STACK_V2_TYPE_SIZE : size;
    stack_v2_ptr stack = CALL(os)->calloc(1, STACK_V2_TYPE_SIZE);
    stack->bp = CALL(os)->calloc(default_size, PTR_SIZE);
    stack->sp = stack->bp + default_size;
    stack->next = next;
    stack->size = next ? next->size : 0;
    stack->default_size = default_size;
    return stack;
}

/* destroys the memory stack */
static u64 list_destroy(stack_v2_ptr* _stack_ptr) {
    CHECK_ARG(_stack_ptr, FALSE);
    CHECK_ARG(*_stack_ptr, FALSE);
    stack_v2_ptr stack = *_stack_ptr;
    stack_v2_ptr ptr = stack;
    while (ptr != 0) {
        stack_v2_ptr current = ptr;
        ptr = ptr->next;
        CALL(os)->free(current->bp);
        CALL(os)->free(current);
    };
    *_stack_ptr = NULL_PTR;
    return TRUE;
}

/* pushes the memory pointer */
static u64 list_push(stack_v2_ptr* _stack_ptr, void_ptr data) {
    CHECK_ARG(_stack_ptr, FALSE);
    CHECK_ARG(*_stack_ptr, FALSE);
    stack_v2_ptr stack = *_stack_ptr;
    if (stack->sp == stack->bp) {
        stack = *_stack_ptr = list_init(stack->default_size, stack);
    }
    --stack->sp;
    *stack->sp = data;
    stack->size++;
    return TRUE;
}

/* pop existing element at the top of the stack/queue/list */
static void_ptr list_pop(stack_v2_ptr* _stack_ptr) {
    CHECK_ARG(_stack_ptr, NULL_PTR);
    CHECK_ARG(*_stack_ptr, NULL_PTR);
    stack_v2_ptr stack = *_stack_ptr;
    CHECK_VALUE(stack->size, NULL_PTR);
    if (stack->sp == stack->bp + stack->default_size) {
        stack_v2_ptr current = stack->next;
        CALL(os)->free(stack->bp);
        CALL(os)->free(stack);
        stack = *_stack_ptr = current;
    }
    CHECK_VALUE(stack, NULL_PTR);
    void_ptr data = *stack->sp;
    ++stack->sp;
    stack->size--;
    return data;
}

/* peeks existing element at the top of the stack/queue/list */
static void_ptr list_peek(stack_v2_ptr* _stack_ptr) {
    CHECK_ARG(_stack_ptr, NULL_PTR);
    CHECK_ARG(*_stack_ptr, NULL_PTR);
    stack_v2_ptr stack = *_stack_ptr;
    CHECK_VALUE(stack->size, NULL_PTR);
    CHECK_CONDITION(stack->sp == stack->bp + stack->default_size, NULL_PTR);
    void_ptr data = *stack->sp;
    return data;
}

static u64 list_diff(stack_v2_ptr stack1, stack_v2_ptr stack2, stack_v2_ptr* compare) {
    CHECK_ARG(stack1, FALSE);
    CHECK_ARG(stack2, FALSE);
    CHECK_ARG(compare, FALSE);
    list_diff_internal(stack1, stack2, compare, compare);
    return TRUE;
}

static u64 list_diff_left(stack_v2_ptr stack1, stack_v2_ptr stack2, stack_v2_ptr* compare) {
    CHECK_ARG(stack1, FALSE);
    CHECK_ARG(stack2, FALSE);
    CHECK_ARG(compare, FALSE);
    list_diff_internal(stack1, stack2, compare, NULL_PTR);
    return TRUE;
}

static u64 list_diff_right(stack_v2_ptr stack1, stack_v2_ptr stack2, stack_v2_ptr* compare) {
    CHECK_ARG(stack1, FALSE);
    CHECK_ARG(stack2, FALSE);
    CHECK_ARG(compare, FALSE);
    list_diff_internal(stack1, stack2, NULL_PTR, compare);
    return TRUE;
}

/* internal */
static void list_diff_internal(stack_v2_ptr stack1, stack_v2_ptr stack2, stack_v2_ptr* compare1, stack_v2_ptr* compare2) {
    hashtable_ptr ht1 = CALL(hashtable)->init();
    stack_v2_ptr stack1_ptr = stack1;
    u64 stack1_size = stack1->default_size;
    while (stack1_ptr != NULL_PTR && stack1_ptr->sp != stack1_ptr->bp + stack1_size) {
        void_ptr* stack1_current = stack1_ptr->sp;
        while (stack1_current != stack1_ptr->bp + stack1_size) {
            void_ptr ptr1 = *stack1_current;
            if (CALL(hashtable)->get(ht1, (u64)ptr1) == NULL_PTR) {
                CALL(hashtable)->insert(ht1, (u64)ptr1, ptr1);
            }
            ++stack1_current;
        }
        stack1_ptr = stack1_ptr->next;
    }
    hashtable_ptr ht2 = CALL(hashtable)->init();
    stack_v2_ptr stack2_ptr = stack2;
    u64 stack2_size = stack2->default_size;
    while (stack2_ptr != NULL_PTR && stack2_ptr->sp != stack2_ptr->bp + stack2_size) {
        void_ptr* stack2_current = stack2_ptr->sp;
        while (stack2_current != stack2_ptr->bp + stack2_size) {
            void_ptr ptr2 = *stack2_current;
            if (CALL(hashtable)->get(ht2, (u64)ptr2) == NULL_PTR) {
                CALL(hashtable)->insert(ht2, (u64)ptr2, ptr2);
            }
            ++stack2_current;
        }
        stack2_ptr = stack2_ptr->next;
    }
    if (compare1 != 0) {
        stack1_ptr = stack1;
        while (stack1_ptr != NULL_PTR && stack1_ptr->sp != stack1_ptr->bp + stack1_size) {
            void_ptr* stack1_current = stack1_ptr->sp;
            while (stack1_current != stack1_ptr->bp + stack1_size) {
                void_ptr ptr1 = *stack1_current;
                if (CALL(hashtable)->get(ht2, (u64)ptr1) == NULL_PTR) {
                    list_push(compare1, ptr1);
                }
                ++stack1_current;
            }
            stack1_ptr = stack1_ptr->next;
        }
    }
    if (compare2 != 0) {
        stack2_ptr = stack2;
        while (stack2_ptr != NULL_PTR && stack2_ptr->sp != stack2_ptr->bp + stack2_size) {
            void_ptr* stack2_current = stack2_ptr->sp;
            while (stack2_current != stack2_ptr->bp + stack2_size) {
                void_ptr ptr2 = *stack2_current;
                if (CALL(hashtable)->get(ht1, (u64)ptr2) == NULL_PTR) {
                    list_push(compare2, ptr2);
                }
                ++stack2_current;
            }
            stack2_ptr = stack2_ptr->next;
        }
    }
    CALL(hashtable)->destroy(ht1);
    CALL(hashtable)->destroy(ht2);
}

/* public */

const virtual_list_v2_methods PRIVATE_API(virtual_list_v2_methods_definitions) = {
    .init = list_init,
    .push = list_push,
    .pop = list_pop,
    .peek = list_peek,
    .diff = list_diff,
    .diff_left = list_diff_left,
    .diff_right = list_diff_right,
    .destroy = list_destroy,
};

const virtual_list_v2_methods* PRIVATE_API(list_v2) = &PRIVATE_API(virtual_list_v2_methods_definitions);
const virtual_list_v2_methods* CALL(list_v2) {
    return PRIVATE_API(list_v2);
}
