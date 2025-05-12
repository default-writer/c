/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 11, 2025 at 11:45:59 AM GMT+3
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

#include "test_vm.h"

#include "std/macros.h"

#include "system/memory/memory_v1.h"
#include "system/options/options_v1.h"
#include "system/os/os_v1.h"

#include "virtual/env/env_v1.h"
#include "virtual/list/list_v1.h"
#include "virtual/list/list_v2.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/data/data_v1.h"
#include "virtual/types/stack/stack_v1.h"
#include "virtual/types/string/string_v1.h"
#include "virtual/types/user/user_v1.h"
#include "virtual/virtual/virtual_v1.h"
#include "virtual/vm/vm_v1.h"

#include "internal/pointer_type_v1.h"

#define TYPE_USER_STACK (TYPE_USER + 1)

#define USING_TESTS
#include "test.h"

/* Data structure to use at the core of our fixture. */
typedef struct test_data {
    const_vm_ptr ctx;
}* TEST_DATA;

typedef struct stack_handler {
    u64 size;
    stack_ptr list;
} stack_handler_type;

RX_SET_UP(test_set_up) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    rx->ctx = CALL(system)->init(0);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(system)->destroy(cvm);
}

RX_SET_UP(test_clean_set_up) {
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_clean_tear_down) {
    /* nothing to cleanup */
}

RX_SET_UP(test_set_up_pointer_init) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    rx->ctx = CALL(vm)->init(8);
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_tear_down_pointer_destroy) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* internal */
static void parse_text(const_vm_ptr cvm, u64 text_string_ptr);
static void parse_text_memory_leak1(const_vm_ptr cvm, u64 text_string_ptr);
static void parse_text_memory_leak2(const_vm_ptr cvm, u64 text_string_ptr);

/* Define the fixture. */
RX_FIXTURE(test_fixture_pointer, TEST_DATA, .set_up = test_set_up_pointer_init, .tear_down = test_tear_down_pointer_destroy);

/* Define the fixture. */
RX_FIXTURE(test_fixture, TEST_DATA, .set_up = test_set_up, .tear_down = test_tear_down);

/* Define the fixture. */
RX_FIXTURE(test_clean_fixture, TEST_DATA, .set_up = test_clean_set_up, .tear_down = test_clean_tear_down);

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_virtual_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const_pointer_ptr ptr = CALL(allocator)->read(cvm, 0);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_destroy_0, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    CALL(system)->destroy(cvm);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_copy_safe_0, .fixture = test_fixture) {
    const_vm_ptr cvm = 0;
    const char* data = "hello";
    u64 size = strlen(data) + 1;
    u64 virtual_ptr = CALL(pointer)->copy(cvm, data, size, size - 1, TYPE_USER);
    RX_ASSERT(virtual_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_copy_safe, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* data = "hello";
    u64 size = strlen(data) + 1;
    u64 virtual_ptr = CALL(pointer)->copy(cvm, data, 2, size - 2, TYPE_USER);
    RX_ASSERT(virtual_ptr != 0);
    CALL(string)->free(cvm, virtual_ptr);
#ifndef USE_GC
    CALL(user)->free(cvm, virtual_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_copy_safe_release_type_id_error_0, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* data = "hello";
    u64 size = strlen(data) + 1;
    u64 virtual_ptr = CALL(pointer)->copy(cvm, data, 2, size - 2, TYPE_USER);
    RX_ASSERT(virtual_ptr != 0);
#ifndef USE_DYNAMIC_TYPES
    u64 result = CALL(pointer)->release(cvm, 1, 0);
    RX_ASSERT(result == 0);
#endif
    CALL(string)->free(cvm, virtual_ptr);
#ifndef USE_GC
    CALL(user)->free(cvm, virtual_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_copy_safe_release_type_id_error, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* data = "hello";
    u64 size = strlen(data) + 1;
    u64 virtual_ptr = CALL(pointer)->copy(cvm, data, 2, size - 2, TYPE_USER);
    RX_ASSERT(virtual_ptr != 0);
#ifndef USE_DYNAMIC_TYPES
    u64 result = CALL(pointer)->release(cvm, virtual_ptr, TYPE_USER + 1);
    RX_ASSERT(result == 0);
#endif
    CALL(string)->free(cvm, virtual_ptr);
#ifndef USE_GC
    CALL(user)->free(cvm, virtual_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_copy_safe_release_type_id, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const char* data = "hello";
    u64 size = strlen(data) + 1;
    u64 virtual_ptr = CALL(pointer)->copy(cvm, data, 2, size - 2, TYPE_USER);
    RX_ASSERT(virtual_ptr != 0);
#ifndef USE_DYNAMIC_TYPES
    u64 result = CALL(pointer)->release(cvm, virtual_ptr, TYPE_USER);
    RX_ASSERT(result != 0);
#endif
    CALL(string)->free(cvm, virtual_ptr);
#ifndef USE_GC
    CALL(user)->free(cvm, virtual_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_allocator_free, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 result = CALL(allocator)->free(cvm, 1);
    RX_ASSERT(result == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_address, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 virtual_ptr = CALL(allocator)->alloc(cvm, 1, TYPE_DATA);
    u64 address = CALL(options)->size + 1;
    const_pointer_ptr ref = CALL(allocator)->read(cvm, address);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(ref == 0);
    CALL(pointer)->free(cvm, address);
    CALL(pointer)->free(cvm, virtual_ptr);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_ref_address_0_0, .fixture = test_clean_fixture) {
    u64 ref = CALL(pointer)->ref(0, 0);
    RX_ASSERT(ref == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_ref_address_0, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 ref = CALL(pointer)->ref(cvm, 0);
    RX_ASSERT(ref == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_ref_address, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 virtual_ptr = CALL(allocator)->alloc(cvm, 1, TYPE_DATA);
    const_pointer_ptr tmp = CALL(allocator)->read(cvm, virtual_ptr);
    u64 address = tmp->public.address;
    u64 ref = CALL(pointer)->ref(cvm, (u64)tmp);
    RX_ASSERT(ref == 1);
    CALL(pointer)->free(cvm, address);
    CALL(pointer)->free(cvm, virtual_ptr);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_type, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 virtual_ptr = CALL(allocator)->alloc(cvm, 1, TYPE_DATA);
    u64 address = CALL(options)->size + 1;
    u64 type_id = CALL(allocator)->type(cvm, address);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(type_id == 0);
#ifndef USE_GC
    CALL(data)->free(cvm, virtual_ptr);
#endif
    CALL(pointer)->free(cvm, address);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_virtual_free_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(allocator)->free(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_free_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(allocator)->free(cvm, 1);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 type_id = CALL(allocator)->type(cvm, 1);
    RX_ASSERT(type_id == TYPE_NULL);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_type_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const_pointer_ptr ptr = CALL(allocator)->read(cvm, 0);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_init_0, .fixture = test_fixture) {
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_virtual_1, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const_pointer_ptr ptr = CALL(allocator)->read(cvm, 1);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_write_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 ptr = CALL(allocator)->alloc(cvm, 0, 0);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_dump_page_2, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 string_ptr[] = {
        CALL(string)->load(cvm, "string1\n"),
        CALL(string)->load(cvm, "string2\n"),
        CALL(string)->load(cvm, "string3\n"),
        CALL(string)->load(cvm, "string4\n"),
        CALL(string)->load(cvm, "string5\n"),
        CALL(string)->load(cvm, "string6\n"),
        CALL(string)->load(cvm, "string7\n"),
        CALL(string)->load(cvm, "string8\n"),
        CALL(string)->load(cvm, "string9_page2\n"),
    };
    CALL(vm)->dump_ref(cvm);
    u64 length = (sizeof(string_ptr) / sizeof(string_ptr[0]));
    for (u64 i = 0; i < length; i++) {
        CALL(env)->puts(cvm, string_ptr[i]);
    }
#ifndef USE_GC
    for (u64 i = 0; i < length; i++) {
        CALL(string)->free(cvm, string_ptr[i]);
    }
#endif
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_diff, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 src1[] = {
        1, 2, 3, 5
    };
    u64 src2[] = {
        1, 2, 4, 6
    };
    stack_ptr stack1 = CALL(list)->init();
    stack_ptr stack2 = CALL(list)->init();
    stack_ptr compare = CALL(list)->init();
    u64 length = sizeof(src1) / sizeof(src1[0]);
    for (u64 i = 0; i < length; i++) {
        CALL(list)->push(stack1, (void_ptr)src1[i]);
        CALL(list)->push(stack2, (void_ptr)src2[i]);
    }
    CALL(list)->diff(stack1, stack2, compare);
    stack_element_ptr current = compare->current;
    u64 i = 0;
    u64 expected[] = { 4, 6, 3, 5 };
    while (current != NULL_PTR) {
        u64 ptr = (u64)current->data;
        printf("A ^ B: %016llx\n", ptr);
        RX_ASSERT(ptr == expected[i++]);
        current = current->next;
    }
    RX_ASSERT(i == 4);
    CALL(list)->destroy(stack1);
    CALL(list)->destroy(stack2);
    CALL(list)->destroy(compare);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_diff_left, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 src1[] = {
        1, 2, 3, 5
    };
    u64 src2[] = {
        1, 2, 4, 6
    };
    stack_ptr stack1 = CALL(list)->init();
    stack_ptr stack2 = CALL(list)->init();
    stack_ptr compare = CALL(list)->init();
    u64 length = sizeof(src1) / sizeof(src1[0]);
    for (u64 i = 0; i < length; i++) {
        CALL(list)->push(stack1, (void_ptr)src1[i]);
        CALL(list)->push(stack2, (void_ptr)src2[i]);
    }
    CALL(list)->diff_left(stack1, stack2, compare);
    stack_element_ptr current = compare->current;
    u64 i = 0;
    u64 expected[] = { 3, 5 };
    while (current != NULL_PTR) {
        u64 ptr = (u64)current->data;
        printf("A \\ B: %016llx\n", ptr);
        RX_ASSERT(ptr == expected[i++]);
        current = current->next;
    }
    RX_ASSERT(i == 2);
    CALL(list)->destroy(stack1);
    CALL(list)->destroy(stack2);
    CALL(list)->destroy(compare);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_diff_right, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 src1[] = {
        1, 2, 3, 5
    };
    u64 src2[] = {
        1, 2, 4, 6
    };
    stack_ptr stack1 = CALL(list)->init();
    stack_ptr stack2 = CALL(list)->init();
    stack_ptr compare = CALL(list)->init();
    u64 length = sizeof(src1) / sizeof(src1[0]);
    for (u64 i = 0; i < length; i++) {
        CALL(list)->push(stack1, (void_ptr)src1[i]);
        CALL(list)->push(stack2, (void_ptr)src2[i]);
    }
    CALL(list)->diff_right(stack1, stack2, compare);
    stack_element_ptr current = compare->current;
    u64 i = 0;
    u64 expected[] = { 4, 6 };
    while (current != NULL_PTR) {
        u64 ptr = (u64)current->data;
        printf("B \\ A: %016llx\n", ptr);
        RX_ASSERT(ptr == expected[i++]);
        current = current->next;
    }
    RX_ASSERT(i == 2);
    CALL(list)->destroy(stack1);
    CALL(list)->destroy(stack2);
    CALL(list)->destroy(compare);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_v2_init, .fixture = test_clean_fixture) {
    u64 src[] = {
        1, 2, 3, 5
    };
    stack_v2_ptr stack = CALL(list_v2)->init(2, NULL_PTR);
    RX_ASSERT(stack != NULL_PTR);
    CALL(list_v2)->destroy(&stack);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_v2_push_1, .fixture = test_clean_fixture) {
    u64 src[] = {
        1, 2, 3, 5
    };
    stack_v2_ptr stack = CALL(list_v2)->init(2, NULL_PTR);
    u64 result = CALL(list_v2)->push(&stack, (void_ptr)src[0]);
    RX_ASSERT(result == TRUE);
    RX_ASSERT(src[0] == (u64)CALL(list_v2)->pop(&stack));
    CALL(list_v2)->destroy(&stack);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_v2_push_2, .fixture = test_clean_fixture) {
    u64 src[] = {
        1, 2, 3, 5
    };
    stack_v2_ptr stack = CALL(list_v2)->init(2, NULL_PTR);
    CALL(list_v2)->push(&stack, (void_ptr)src[0]);
    u64 result = CALL(list_v2)->push(&stack, (void_ptr)src[1]);
    RX_ASSERT(result == TRUE);
    RX_ASSERT(src[1] == (u64)CALL(list_v2)->pop(&stack));
    RX_ASSERT(src[0] == (u64)CALL(list_v2)->pop(&stack));
    CALL(list_v2)->destroy(&stack);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_v2_push_3, .fixture = test_clean_fixture) {
    u64 src[] = {
        1, 2, 3, 5
    };
    stack_v2_ptr stack = CALL(list_v2)->init(2, NULL_PTR);
    CALL(list_v2)->push(&stack, (void_ptr)src[0]);
    CALL(list_v2)->push(&stack, (void_ptr)src[1]);
    u64 result = CALL(list_v2)->push(&stack, (void_ptr)src[2]);
    RX_ASSERT(result == TRUE);
    RX_ASSERT(src[2] == (u64)CALL(list_v2)->pop(&stack));
    RX_ASSERT(src[1] == (u64)CALL(list_v2)->pop(&stack));
    RX_ASSERT(src[0] == (u64)CALL(list_v2)->pop(&stack));
    CALL(list_v2)->destroy(&stack);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_v2_push_4, .fixture = test_clean_fixture) {
    u64 src[] = {
        1, 2, 3, 5
    };
    stack_v2_ptr stack = CALL(list_v2)->init(2, NULL_PTR);
    CALL(list_v2)->push(&stack, (void_ptr)src[0]);
    CALL(list_v2)->push(&stack, (void_ptr)src[1]);
    CALL(list_v2)->push(&stack, (void_ptr)src[2]);
    u64 result = CALL(list_v2)->push(&stack, (void_ptr)src[3]);
    RX_ASSERT(result == TRUE);
    RX_ASSERT(src[3] == (u64)CALL(list_v2)->pop(&stack));
    RX_ASSERT(src[2] == (u64)CALL(list_v2)->pop(&stack));
    RX_ASSERT(src[1] == (u64)CALL(list_v2)->pop(&stack));
    RX_ASSERT(src[0] == (u64)CALL(list_v2)->pop(&stack));
    CALL(list_v2)->destroy(&stack);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_v2_push_5, .fixture = test_clean_fixture) {
    u64 src[] = {
        1, 2, 3, 5, 9
    };
    stack_v2_ptr stack = CALL(list_v2)->init(0, NULL_PTR);
    CALL(list_v2)->push(&stack, (void_ptr)src[0]);
    CALL(list_v2)->push(&stack, (void_ptr)src[1]);
    CALL(list_v2)->push(&stack, (void_ptr)src[2]);
    CALL(list_v2)->push(&stack, (void_ptr)src[3]);
    u64 result = CALL(list_v2)->push(&stack, (void_ptr)src[4]);
    RX_ASSERT(result == TRUE);
    RX_ASSERT(src[4] == (u64)CALL(list_v2)->pop(&stack));
    RX_ASSERT(src[3] == (u64)CALL(list_v2)->pop(&stack));
    RX_ASSERT(src[2] == (u64)CALL(list_v2)->pop(&stack));
    RX_ASSERT(src[1] == (u64)CALL(list_v2)->pop(&stack));
    RX_ASSERT(src[0] == (u64)CALL(list_v2)->pop(&stack));
    CALL(list_v2)->destroy(&stack);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_v2_peek_empty, .fixture = test_clean_fixture) {
    u64 src[] = {
        1, 2, 3, 5
    };
    stack_v2_ptr stack = CALL(list_v2)->init(0, NULL_PTR);
    void_ptr value = CALL(list_v2)->peek(&stack);
    RX_ASSERT(value == NULL_PTR);
    CALL(list_v2)->pop(&stack);
    CALL(list_v2)->destroy(&stack);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_v2_push_peek, .fixture = test_clean_fixture) {
    u64 src[] = {
        1, 2, 3, 5
    };
    stack_v2_ptr stack = CALL(list_v2)->init(0, NULL_PTR);
    CALL(list_v2)->push(&stack, (void_ptr)src[0]);
    u64 value = (u64)CALL(list_v2)->peek(&stack);
    RX_ASSERT(value == src[0]);
    CALL(list_v2)->pop(&stack);
    CALL(list_v2)->destroy(&stack);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_v2_diff, .fixture = test_clean_fixture) {
    u64 src1[] = {
        1, 2, 3, 5
    };
    u64 src2[] = {
        1, 2, 4, 6
    };
    stack_v2_ptr stack1 = CALL(list_v2)->init(2, NULL_PTR);
    stack_v2_ptr stack2 = CALL(list_v2)->init(2, NULL_PTR);
    stack_v2_ptr compare = CALL(list_v2)->init(2, NULL_PTR);
    u64 length = sizeof(src1) / sizeof(src1[0]);
    for (u64 i = 0; i < length; i++) {
        CALL(list_v2)->push(&stack1, (void_ptr)src1[i]);
        CALL(list_v2)->push(&stack2, (void_ptr)src2[i]);
    }
    CALL(list_v2)->diff(stack1, stack2, &compare);
    stack_v2_ptr _stack_ptr = compare;
    u64 i = 0;
    u64 expected[] = { 4, 6, 3, 5 };
    u64 stack_size = compare->default_size;
    while (_stack_ptr != NULL_PTR && _stack_ptr->sp != _stack_ptr->bp + stack_size) {
        void_ptr* stack_current = _stack_ptr->sp;
        while (stack_current != _stack_ptr->bp + stack_size) {
            void_ptr ptr = *stack_current;
            printf("A \\ B: %016llx\n", (u64)ptr);
            RX_ASSERT((u64)ptr == expected[i++]);
            ++stack_current;
        }
        _stack_ptr = _stack_ptr->next;
    }
    RX_ASSERT(i == 4);
    CALL(list_v2)->destroy(&stack1);
    CALL(list_v2)->destroy(&stack2);
    CALL(list_v2)->destroy(&compare);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_v2_diff_left, .fixture = test_clean_fixture) {
    u64 src1[] = {
        1, 2, 3, 5
    };
    u64 src2[] = {
        1, 2, 4, 6
    };
    stack_v2_ptr stack1 = CALL(list_v2)->init(0, NULL_PTR);
    stack_v2_ptr stack2 = CALL(list_v2)->init(0, NULL_PTR);
    stack_v2_ptr compare = CALL(list_v2)->init(0, NULL_PTR);
    u64 length = sizeof(src1) / sizeof(src1[0]);
    for (u64 i = 0; i < length; i++) {
        CALL(list_v2)->push(&stack1, (void_ptr)src1[i]);
        CALL(list_v2)->push(&stack2, (void_ptr)src2[i]);
    }
    CALL(list_v2)->diff_left(stack1, stack2, &compare);
    stack_v2_ptr _stack_ptr = compare;
    u64 i = 0;
    u64 expected[] = { 3, 5 };
    u64 stack_size = compare->default_size;
    while (_stack_ptr != NULL_PTR && _stack_ptr->sp != _stack_ptr->bp + stack_size) {
        void_ptr* stack_current = _stack_ptr->sp;
        while (stack_current != _stack_ptr->bp + stack_size) {
            void_ptr ptr = *stack_current;
            printf("A \\ B: %016llx\n", (u64)ptr);
            RX_ASSERT((u64)ptr == expected[i++]);
            ++stack_current;
        }
        _stack_ptr = _stack_ptr->next;
    }
    CALL(list_v2)->destroy(&stack1);
    CALL(list_v2)->destroy(&stack2);
    CALL(list_v2)->destroy(&compare);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_v2_diff_right, .fixture = test_clean_fixture) {
    u64 src1[] = {
        1, 2, 3, 5
    };
    u64 src2[] = {
        1, 2, 4, 6
    };
    stack_v2_ptr stack1 = CALL(list_v2)->init(0, NULL_PTR);
    stack_v2_ptr stack2 = CALL(list_v2)->init(0, NULL_PTR);
    stack_v2_ptr compare = CALL(list_v2)->init(0, NULL_PTR);
    u64 length = sizeof(src1) / sizeof(src1[0]);
    for (u64 i = 0; i < length; i++) {
        CALL(list_v2)->push(&stack1, (void_ptr)src1[i]);
        CALL(list_v2)->push(&stack2, (void_ptr)src2[i]);
    }
    CALL(list_v2)->diff_right(stack1, stack2, &compare);
    stack_v2_ptr _stack_ptr = compare;
    u64 i = 0;
    u64 expected[] = { 4, 6 };
    u64 stack_size = compare->default_size;
    while (_stack_ptr != NULL_PTR && _stack_ptr->sp != _stack_ptr->bp + stack_size) {
        void_ptr* stack_current = _stack_ptr->sp;
        while (stack_current != _stack_ptr->bp + stack_size) {
            void_ptr ptr = *stack_current;
            printf("A \\ B: %016llx\n", (u64)ptr);
            RX_ASSERT((u64)ptr == expected[i++]);
            ++stack_current;
        }
        _stack_ptr = _stack_ptr->next;
    }
    CALL(list_v2)->destroy(&stack1);
    CALL(list_v2)->destroy(&stack2);
    CALL(list_v2)->destroy(&compare);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_dump, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 text_string_ptr[] = {
        CALL(string)->load(cvm, "a\nb"),
        CALL(string)->load(cvm, "abc\nabcd\nbcde\nabc\na\nb"),
        CALL(string)->load(cvm, "ab\nabc\n")
    };
    parse_text(cvm, text_string_ptr[1]);
    CALL(vm)->dump_ref(cvm);
    const_vm_ptr debug_cvm = CALL(vm)->init(8);
    u64 text_size = CALL(string)->size(cvm, text_string_ptr[1]);
    const_void_ptr data = CALL(string)->unsafe(cvm, text_string_ptr[1]);
    u64 debug_text_string_ptr = CALL(pointer)->copy(debug_cvm, data, text_size + 1, 0, TYPE_STRING);
    parse_text(debug_cvm, debug_text_string_ptr);
    CALL(vm)->dump_ref(debug_cvm);
#ifndef USE_GC
    for (u64 i = 0; i < sizeof(text_string_ptr) / sizeof(text_string_ptr[0]); i++) {
        CALL(string)->free(cvm, text_string_ptr[i]);
    }
    CALL(string)->free(debug_cvm, debug_text_string_ptr);
#else
    CALL(vm)->gc(debug_cvm);
    CALL(vm)->gc(cvm);
#endif
    CALL(vm)->destroy(debug_cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_dump_stack, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 text_string_ptr[] = {
        CALL(string)->load(cvm, "a\nb"),
        CALL(string)->load(cvm, "abc\nabcd\nbcde\nabc\na\nb"),
        CALL(string)->load(cvm, "ab\nabc\n")
    };
    parse_text(cvm, text_string_ptr[1]);
    stack_ptr stack = CALL(list)->init();
    CALL(vm)->dump_ref_stack(cvm, stack);
    const_vm_ptr debug_cvm = CALL(vm)->init(8);
    u64 text_size = CALL(string)->size(cvm, text_string_ptr[1]);
    const_void_ptr data = CALL(string)->unsafe(cvm, text_string_ptr[1]);
    u64 debug_text_string_ptr = CALL(pointer)->copy(debug_cvm, data, text_size + 1, 0, TYPE_STRING);
    parse_text(debug_cvm, debug_text_string_ptr);
    stack_ptr debug_stack = CALL(list)->init();
    CALL(vm)->dump_ref_stack(debug_cvm, debug_stack);
#ifndef USE_GC
    for (u64 i = 0; i < sizeof(text_string_ptr) / sizeof(text_string_ptr[0]); i++) {
        CALL(string)->free(cvm, text_string_ptr[i]);
    }
    CALL(string)->free(debug_cvm, debug_text_string_ptr);
#else
    CALL(vm)->gc(debug_cvm);
    CALL(vm)->gc(cvm);
#endif
    CALL(list)->destroy(debug_stack);
    CALL(list)->destroy(stack);
    CALL(vm)->destroy(debug_cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_dump_ref, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 text_string_ptr[] = {
        CALL(string)->load(cvm, "a\nb"),
        CALL(string)->load(cvm, "abc\nabcd\nbcde\nabc\na\nb"),
        CALL(string)->load(cvm, "ab\nabc\n")
    };
    for (u64 i = 0; i < sizeof(text_string_ptr) / sizeof(text_string_ptr[0]); i++) {
        CALL(env)->puts(cvm, text_string_ptr[i]);
        parse_text(cvm, text_string_ptr[i]);
    }
    CALL(vm)->dump_ref(cvm);
    const_vm_ptr debug_cvm = CALL(vm)->init(8);
    u64 text_size = CALL(string)->size(cvm, text_string_ptr[1]);
    const_void_ptr data = CALL(string)->unsafe(cvm, text_string_ptr[1]);
    u64 debug_text_string_ptr = CALL(pointer)->copy(debug_cvm, data, text_size + 1, 0, TYPE_STRING);
    parse_text(debug_cvm, debug_text_string_ptr);
    CALL(vm)->dump_ref(debug_cvm);
#ifndef USE_GC
    for (u64 i = 0; i < sizeof(text_string_ptr) / sizeof(text_string_ptr[0]); i++) {
        CALL(string)->free(cvm, text_string_ptr[i]);
    }
    CALL(string)->free(debug_cvm, debug_text_string_ptr);
#else
    CALL(vm)->gc(debug_cvm);
    CALL(vm)->gc(cvm);
#endif
    CALL(vm)->destroy(debug_cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_dump_ref_stack, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 text_string_ptr[] = {
        CALL(string)->load(cvm, "a\nb"),
        CALL(string)->load(cvm, "abc\nabcd\nbcde\nabc\na\nb"),
        CALL(string)->load(cvm, "ab\nabc\n")
    };
    parse_text(cvm, text_string_ptr[1]);
    stack_ptr stack = CALL(list)->init();
    CALL(vm)->dump_ref_stack(cvm, stack);
    const_vm_ptr debug_cvm = CALL(vm)->init(8);
    u64 text_size = CALL(string)->size(cvm, text_string_ptr[1]);
    const_void_ptr data = CALL(string)->unsafe(cvm, text_string_ptr[1]);
    u64 debug_text_string_ptr = CALL(pointer)->copy(debug_cvm, data, text_size + 1, 0, TYPE_STRING);
    parse_text(debug_cvm, debug_text_string_ptr);
    stack_ptr debug_stack = CALL(list)->init();
    CALL(vm)->dump_ref_stack(debug_cvm, debug_stack);
#ifndef USE_GC
    for (u64 i = 0; i < sizeof(text_string_ptr) / sizeof(text_string_ptr[0]); i++) {
        CALL(string)->free(cvm, text_string_ptr[i]);
    }
    CALL(string)->free(debug_cvm, debug_text_string_ptr);
#else
    CALL(vm)->gc(debug_cvm);
    CALL(vm)->gc(cvm);
#endif
    CALL(list)->destroy(debug_stack);
    CALL(list)->destroy(stack);
    CALL(vm)->destroy(debug_cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_dump_memory_leak_1, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    const char* test_data[] = {
        0,
        "a",
        "a\nb",
        "ab\nabc\n",
        "adadadsadsadad\ndad\nadsaddasaddad\nsad\n",
        "ab\ndad\nabcd\nbcd\n",
        "ab\nc\nabc\nbcd\n",
        "abc\nabcd\nbcde\nabc\n",
        "abc\n\n"
    };
    const_vm_ptr debug_cvm = CALL(vm)->init(8);
    for (u64 i = 0; i < sizeof(test_data) / sizeof(test_data[0]); i++) {

        // change to patched original version (parse_text) to see that it will not raise any memory leaks
        // leaks happens due to the fact that data cointaining in a pointer reference is lost forever
        // when it happens, you will not able to recover and free allocated memory for the .data pointer
        // you simply lost reference to the memory pointer you have to keep it when processing data

        u64 debug_text_string_ptr = CALL(string)->load(debug_cvm, test_data[i]);
        parse_text_memory_leak1(debug_cvm, debug_text_string_ptr);
        CALL(string)->free(debug_cvm, debug_text_string_ptr);

        CALL(vm)->dump_ref(debug_cvm);
        stack_ptr debug_cvm_stack = CALL(list)->init();
        CALL(vm)->dump_ref_stack(debug_cvm, debug_cvm_stack);

        u64 text_string_ptr = CALL(string)->load(cvm, test_data[i]);
        parse_text(cvm, text_string_ptr);
        CALL(string)->free(cvm, text_string_ptr);

        CALL(vm)->dump_ref(cvm);
        stack_ptr cvm_stack = CALL(list)->init();
        CALL(vm)->dump_ref_stack(cvm, cvm_stack);

        // get unmatched pointers (it is possible to got memory leaks in both A and B memory dumps)
        // in this particual scenario we can safely assume memory leaks are in (A \ B) set

        stack_ptr compare_left = CALL(list)->init();
        CALL(list)->diff_left(debug_cvm_stack, cvm_stack, compare_left);
        stack_element_ptr current = 0;
        current = compare_left->current;
        while (current != NULL_PTR) {
            u64 ptr = (u64)current->data;
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
            const char* start = "\x1b[34m";
            const char* end = "\x1b[0m";
            fprintf(stderr, "%s[  v< ]%s: %016llx\n", start, end, ptr); /* NOLINT */
#else
            fprintf(stderr, "  v< : %016llx\n", ptr); /* NOLINT */
#endif
#endif
            const_pointer_ptr const_ptr = (const_pointer_ptr)ptr;
            CALL(pointer)->free(debug_cvm, const_ptr->public.address);
            current = current->next;
        }
        CALL(list)->destroy(debug_cvm_stack);
        CALL(list)->destroy(cvm_stack);
        CALL(list)->destroy(compare_left);
    }
#ifdef USE_GC
    CALL(vm)->gc(debug_cvm);
    CALL(vm)->gc(cvm);
#endif
    CALL(vm)->destroy(debug_cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_dump_memory_leak_2, .fixture = test_clean_fixture) {
    const char* test_data[] = {
        "a",
        "a\nb",
        "ab\nabc\n",
        "adadadsadsadad\ndad\nadsaddasaddad\nsad\n",
        "ab\ndad\nabcd\nbcd\n",
        "ab\nc\nabc\nbcd\n",
        "abc\nabcd\nbcde\nabc\n",
        "abc\n\n"
    };
    const_vm_ptr debug_cvm = CALL(vm)->init(8);
    for (u64 i = 0; i < sizeof(test_data) / sizeof(test_data[0]); i++) {

        u64 debug_text_string_ptr = CALL(string)->load(debug_cvm, test_data[i]);
        parse_text_memory_leak2(debug_cvm, debug_text_string_ptr);
        CALL(string)->free(debug_cvm, debug_text_string_ptr);

        stack_ptr debug_cvm_stack = CALL(list)->init();
        CALL(vm)->dump_ref_stack(debug_cvm, debug_cvm_stack);

        u64 current = 0;
        while ((current = (u64)CALL(list)->pop(debug_cvm_stack)) != 0) {
            u64 ptr = current;
            const_pointer_ptr const_ptr = (const_pointer_ptr)ptr;
            u64 address = const_ptr->public.address;
#ifdef USE_MEMORY_DEBUG_INFO
#ifdef USE_TTY
            const char* start = "\x1b[34m";
            const char* end = "\x1b[0m";
            fprintf(stderr, "%s[  v< ]%s: %016llx\n", start, end, ptr); /* NOLINT */
            fprintf(stderr, "%s[  v& ]%s: %016llx\n", start, end, address); /* NOLINT */
#else
            fprintf(stderr, "  v< : %016llx\n", ptr); /* NOLINT */
            fprintf(stderr, "  v& : %016llx\n", address); /* NOLINT */
#endif
#endif
            CALL(pointer)->free(debug_cvm, address);
        }
        CALL(list)->destroy(debug_cvm_stack);
    }
#ifdef USE_GC
    CALL(vm)->gc(debug_cvm);
#endif
    CALL(vm)->destroy(debug_cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_dump_ref_0, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    stack_ptr stack = 0;
    CALL(vm)->dump_ref(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_dump_ref_stack_0, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    stack_ptr stack = (stack_ptr)CALL(list)->init();
    CALL(vm)->dump_ref_stack(cvm, stack);
    void_ptr current = (void_ptr)CALL(list)->peek(stack);
    RX_ASSERT(current == 0);
#ifndef USE_GC
#else
    CALL(vm)->gc(cvm);
#endif
    CALL(list)->destroy(stack);
    CALL(vm)->destroy(cvm);
}

/* -------------------------------------------------------- *
 *                                                          *
 *                                                          *
 * test pointer                                             *
 *                                                          *
 *                                                          *
 * -------------------------------------------------------- */

typedef struct test_pointer_data {
    void_ptr ptr;
}* TEST_POINTER_DATA;

RX_SET_UP(test_pointer_set_up) {
    return RX_SUCCESS;
}

RX_TEAR_DOWN(test_pointer_tear_down) {
    /* nothing to cleanup */
}

/* Define the fixture. */
RX_FIXTURE(test_pointer_fixture, TEST_POINTER_DATA, .set_up = test_pointer_set_up, .tear_down = test_pointer_tear_down);

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 list_ptr = CALL(stack)->alloc(cvm);
    CALL(stack)->free(cvm, list_ptr);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_user, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 user_ptr = CALL(user)->alloc(cvm);
    CALL(stack)->push(cvm, list_ptr, user_ptr);
    CALL(user)->free(cvm, user_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_data, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 data_ptr = CALL(data)->alloc(cvm, 1024);
    CALL(stack)->push(cvm, list_ptr, data_ptr);
    CALL(data)->free(cvm, data_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_free_user_free, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 list_ptr = CALL(stack)->alloc(cvm);
    u64 user_ptr = CALL(user)->alloc(cvm);
    CALL(stack)->push(cvm, list_ptr, user_ptr);
    CALL(stack)->free(cvm, list_ptr);
    CALL(user)->free(cvm, user_ptr);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_user_free_0, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    CALL(user)->free(cvm, 0);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_virtual_alloc_release, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(allocator)->alloc(cvm, 8, TYPE_USER + 1);
    const_pointer_ptr data_ptr = CALL(allocator)->read(cvm, ptr_id);
    RX_ASSERT(data_ptr != 0);
    CALL(pointer)->free(cvm, ptr_id);
    RX_ASSERT(data_ptr != 0);
    CALL(vm)->release(cvm, 0);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_gc_destroy_free_unsafe, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(allocator)->alloc(cvm, 8, TYPE_DATA);
    CALL(vm)->gc(cvm);
#ifndef USE_GC
    CALL(data)->free(cvm, ptr_id);
#endif
    CALL(vm)->destroy(cvm);
    CALL(vm)->release(cvm, ptr_id);
    CALL(data)->free(cvm, ptr_id);
    void_ptr u = CALL(data)->unsafe(cvm, ptr_id);
    RX_ASSERT(u == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_release_0gc_destroy_read_type, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(allocator)->alloc(cvm, 8, TYPE_DATA);
    CALL(pointer)->free(cvm, ptr_id);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
    const_pointer_ptr data_ptr = CALL(allocator)->read(cvm, ptr_id);
    RX_ASSERT(data_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_gc_destroy_read_type_release_0, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(allocator)->alloc(cvm, 8, TYPE_DATA);
#ifndef USE_GC
    CALL(pointer)->free(cvm, ptr_id);
#endif
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
    const_pointer_ptr data_ptr = CALL(allocator)->read(cvm, ptr_id);
    RX_ASSERT(data_ptr == 0);
    CALL(pointer)->free(cvm, ptr_id);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_destroy_read_type_release_0, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(allocator)->alloc(cvm, 8, TYPE_DATA);
    CALL(vm)->release(cvm, ptr_id);
#ifndef USE_GC
    CALL(pointer)->free(cvm, ptr_id);
#endif
    CALL(vm)->destroy(cvm);
    const_pointer_ptr data_ptr = CALL(allocator)->read(cvm, ptr_id);
    RX_ASSERT(data_ptr == 0);
    CALL(pointer)->free(cvm, ptr_id);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_alloc_ref_write_gc_destroy_read_type_release_0, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(allocator)->alloc(cvm, 8, TYPE_DATA);
#ifndef USE_GC
    CALL(pointer)->free(cvm, ptr_id);
#endif
    CALL(vm)->release(cvm, ptr_id);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
    const_pointer_ptr data_ptr = CALL(allocator)->read(cvm, ptr_id);
    RX_ASSERT(data_ptr == 0);
    CALL(pointer)->free(cvm, ptr_id);
}

static void run(void) {
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- acceptance test code\n");
#endif
    /* safe */
#ifdef USE_MEMORY_DEBUG_INFO
    printf("---- rexo unit test code %s\n", __FILE__);
#endif
}

static void parse_text(const_vm_ptr cvm, u64 text_string_ptr) {
    u64 gc_ptr = CALL(stack)->alloc(cvm);
    u64 text_size = CALL(string)->size(cvm, text_string_ptr);
    if (text_string_ptr == 0 || text_size == 0) {
        CALL(stack)->free(cvm, gc_ptr);
        return;
    }
    u64 stack_ptr1 = CALL(stack)->alloc(cvm);
    char* text = CALL(string)->unsafe(cvm, text_string_ptr);
    char* tmp = text;
    // CALL(string)->split(stack_ptr1);
    while (text != 0 && *tmp != 0 && text_size > 0) {
        while (*tmp != 0 && *tmp != '\n' && text_size > 0) {
            tmp++;
            text_size--;
        }
        if (text_size == 0) {
            u64 string_ptr = 0;
            while ((string_ptr = CALL(stack)->pop(cvm, stack_ptr1)) != 0) {
                CALL(string)->free(cvm, string_ptr);
            }
            CALL(stack)->free(cvm, stack_ptr1);
            CALL(stack)->free(cvm, gc_ptr);
            return;
        }
        *tmp++ = '\0';
        text_size--;
        u64 string_ptr = CALL(string)->load(cvm, text);
        CALL(stack)->push(cvm, stack_ptr1, string_ptr);
        text = tmp;
    }
    u64 data_ptr = 0;
    u64 stack_ptr2 = CALL(stack)->alloc(cvm);
    while ((data_ptr = CALL(stack)->pop(cvm, stack_ptr1)) != 0) {
        CALL(stack)->push(cvm, stack_ptr2, data_ptr);
    }
    CALL(stack)->free(cvm, stack_ptr1);
    CALL(stack)->push(cvm, gc_ptr, stack_ptr2);
    u64 quit = 0;
    while (quit == 0) {
        u64 string_ptr = CALL(stack)->pop(cvm, stack_ptr2);
        if (string_ptr == 0) {
            quit = 1;
            continue;
        }
        CALL(env)->puts(cvm, string_ptr);
        u64 pattern_ptr = CALL(stack)->pop(cvm, stack_ptr2);
        if (CALL(string)->size(cvm, pattern_ptr) == 0) {
            quit = 1;
            CALL(string)->free(cvm, string_ptr);
            CALL(string)->free(cvm, pattern_ptr);
            continue;
        }
        CALL(env)->puts(cvm, pattern_ptr);
        u64 size = CALL(string)->size(cvm, pattern_ptr);
        u64 string_reference_ptr = 0;
        u64 current_ptr = string_ptr;
        while ((string_reference_ptr = CALL(string)->strchr(cvm, current_ptr, pattern_ptr)) != 0) {
            u64 match_ptr = CALL(string)->match(cvm, string_reference_ptr, pattern_ptr);
            if (match_ptr == 0) {
                CALL(string)->free(cvm, string_reference_ptr);
                CALL(string)->free(cvm, string_ptr);
                CALL(string)->free(cvm, pattern_ptr);
                break;
            }
            if (CALL(string)->lessthan(cvm, string_reference_ptr, match_ptr)) {
                u64 match_start_ptr = CALL(string)->left(cvm, match_ptr, size);
                u64 str_ncpy = CALL(string)->right_copy(cvm, match_start_ptr, size);
                u64 distance = CALL(string)->lessthan(cvm, string_ptr, match_start_ptr);
                if (distance > 0) {
                    u64 i = 0;
                    while (i++ < distance) {
                        printf(" ");
                    }
                }
                printf("%s[%lld]\n", CALL(string)->unsafe(cvm, str_ncpy), distance);
                CALL(string)->free(cvm, match_start_ptr);
                CALL(string)->free(cvm, str_ncpy);
            }
            CALL(string)->free(cvm, string_reference_ptr);
            CALL(string)->free(cvm, current_ptr);
            current_ptr = match_ptr;
        }
#ifndef USE_GC
        CALL(string)->free(cvm, string_ptr);
        CALL(string)->free(cvm, pattern_ptr);
        CALL(string)->free(cvm, current_ptr);
#endif
    }
#ifndef USE_GC
    CALL(stack)->free(cvm, stack_ptr2);
#endif
    CALL(stack)->free(cvm, gc_ptr);
}

static void parse_text_memory_leak1(const_vm_ptr cvm, u64 text_string_ptr) {
    u64 gc_ptr = CALL(stack)->alloc(cvm);
    u64 text_size = CALL(string)->size(cvm, text_string_ptr);
    if (text_string_ptr == 0) {
        CALL(stack)->free(cvm, gc_ptr);
        return;
    }
    u64 stack_ptr1 = CALL(stack)->alloc(cvm);
    char* text = CALL(string)->unsafe(cvm, text_string_ptr);
    char* tmp = text;
    while (text != 0 && *tmp != 0 && text_size > 0) {
        while (*tmp != 0 && *tmp != '\n' && text_size > 0) {
            tmp++;
            text_size--;
        }
        if (text_size == 0) {
            u64 string_ptr = 0;
            while ((string_ptr = CALL(stack)->pop(cvm, stack_ptr1)) != 0) {
                CALL(string)->free(cvm, string_ptr);
            }
            CALL(stack)->free(cvm, stack_ptr1);
            CALL(stack)->free(cvm, gc_ptr);
            return;
        }
        *tmp++ = '\0';
        text_size--;
        u64 string_ptr = CALL(string)->load(cvm, text);
        CALL(stack)->push(cvm, stack_ptr1, string_ptr);
        text = tmp;
    }
    u64 data_ptr = 0;
    u64 stack_ptr2 = CALL(stack)->alloc(cvm);
    while ((data_ptr = CALL(stack)->pop(cvm, stack_ptr1)) != 0) {
        CALL(stack)->push(cvm, stack_ptr2, data_ptr);
    }
    CALL(stack)->free(cvm, stack_ptr1);
    CALL(stack)->push(cvm, gc_ptr, stack_ptr2);
    u64 quit = 0;
    while (quit == 0) {
        u64 string_ptr = CALL(stack)->pop(cvm, stack_ptr2);
        if (CALL(string)->size(cvm, string_ptr) == 0) {
            quit = 1;
            CALL(string)->free(cvm, string_ptr);
            continue;
        }
        CALL(env)->puts(cvm, string_ptr);
        u64 pattern_ptr = CALL(stack)->pop(cvm, stack_ptr2);
        if (CALL(string)->size(cvm, pattern_ptr) == 0) {
            quit = 1;
            CALL(string)->free(cvm, string_ptr);
            CALL(string)->free(cvm, pattern_ptr);
            continue;
        }
        CALL(env)->puts(cvm, pattern_ptr);
        u64 size = CALL(string)->size(cvm, pattern_ptr);
        u64 string_reference_ptr = 0;
        u64 current_ptr = string_ptr;
        while ((string_reference_ptr = CALL(string)->strchr(cvm, current_ptr, pattern_ptr)) != 0) {
            u64 match_ptr = CALL(string)->match(cvm, string_reference_ptr, pattern_ptr);
            if (match_ptr == 0) {
                // CALL(string)->free(cvm, string_reference_ptr);
                CALL(string)->free(cvm, string_ptr);
                CALL(string)->free(cvm, pattern_ptr);
                break;
            }
            if (CALL(string)->lessthan(cvm, string_reference_ptr, match_ptr)) {
                u64 match_start_ptr = CALL(string)->left(cvm, match_ptr, size);
                u64 str_ncpy = CALL(string)->right_copy(cvm, match_start_ptr, size);
                u64 distance = CALL(string)->lessthan(cvm, string_ptr, match_start_ptr);
                if (distance > 0) {
                    u64 i = 0;
                    while (i++ < distance) {
                        printf(" ");
                    }
                }
                printf("%s[%lld]\n", CALL(string)->unsafe(cvm, str_ncpy), distance);
                CALL(string)->free(cvm, match_start_ptr);
                CALL(string)->free(cvm, str_ncpy);
            }
            CALL(string)->free(cvm, string_reference_ptr);
            current_ptr = match_ptr;
        }
    }
#ifndef USE_GC
    CALL(stack)->free(cvm, stack_ptr2);
#endif
    CALL(stack)->free(cvm, gc_ptr);
}

static void parse_text_memory_leak2(const_vm_ptr cvm, u64 text_string_ptr) {
    stack_ptr stack_ptr1 = CALL(list)->init();
    if (CALL(string)->split(cvm, text_string_ptr, stack_ptr1) == FALSE) {
        u64 string_ptr;
        while ((string_ptr = (u64)CALL(list)->pop(stack_ptr1)) != 0) {
            CALL(string)->free(cvm, string_ptr);
        }
        CALL(list)->destroy(stack_ptr1);
        return;
    }
    void_ptr data_ptr = 0;
    stack_ptr stack_ptr2 = CALL(list)->init();
    while ((data_ptr = CALL(list)->pop(stack_ptr1)) != 0) {
        CALL(list)->push(stack_ptr2, data_ptr);
    }
    CALL(list)->destroy(stack_ptr1);
    u64 quit = 0;
    while (quit == 0) {
        u64 string_ptr = (u64)CALL(list)->pop(stack_ptr2);
        if (string_ptr == 0 || CALL(string)->size(cvm, string_ptr) == 0) {
            quit = 1;
            continue;
        }
        CALL(env)->puts(cvm, string_ptr);
        u64 pattern_ptr = (u64)CALL(list)->pop(stack_ptr2);
        if (string_ptr == 0 || CALL(string)->size(cvm, pattern_ptr) == 0) {
            quit = 1;
            continue;
        }
        CALL(env)->puts(cvm, pattern_ptr);
        u64 size = CALL(string)->size(cvm, pattern_ptr);
        u64 string_reference_ptr = 0;
        u64 current_ptr = string_ptr;
        while ((string_reference_ptr = CALL(string)->strchr(cvm, current_ptr, pattern_ptr)) != 0) {
            u64 match_ptr = CALL(string)->match(cvm, string_reference_ptr, pattern_ptr);
            if (match_ptr == 0) {
                break;
            }
            if (CALL(string)->lessthan(cvm, string_reference_ptr, match_ptr)) {
                u64 match_start_ptr = CALL(string)->left(cvm, match_ptr, size);
                u64 str_ncpy = CALL(string)->right_copy(cvm, match_start_ptr, size);
                if (str_ncpy != 0) {
                    u64 distance = CALL(string)->lessthan(cvm, string_ptr, match_start_ptr);
                    if (distance > 0) {
                        u64 i = 0;
                        while (i++ < distance) {
                            printf(" ");
                        }
                    }
                    printf("%s[%lld]\n", CALL(string)->unsafe(cvm, str_ncpy), distance);
                    CALL(string)->free(cvm, str_ncpy);
                }
            }
            // CALL(string)->free(cvm, current_ptr);
            current_ptr = match_ptr;
        }
        CALL(string)->free(cvm, current_ptr);
    }
    CALL(list)->destroy(stack_ptr2);
}

const tests_vm_test_suite PRIVATE_API(tests_vm_test_suite_definitions) = {
    .run = run
};
