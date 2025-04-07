/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 7, 2025 at 7:35:59 PM GMT+3
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

#include "test_vm.h"

#include "std/macros.h"

#include "system/list/list_v1.h"
#include "system/memory/memory_v1.h"
#include "system/options/options_v1.h"
#include "system/os/os_v1.h"

#include "virtual/env/env_v1.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/data/data_v1.h"
#include "virtual/types/stack/stack_v1.h"
#include "virtual/types/string/string_v1.h"
#include "virtual/types/string_pointer/string_pointer_v1.h"
#include "virtual/types/user/user_v1.h"
#include "virtual/virtual/virtual_v1.h"
#include "virtual/vm/vm_v1.h"

#include "internal/pointer_type_v1.h"
#include "internal/string_reference_type_v1.h"

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
    const_pointer_ptr ptr = CALL(virtual)->read(cvm, 0);
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
    u64 virtual_ptr = CALL(pointer)->copy(cvm, data, size, size - 1, TYPE_USER);
    RX_ASSERT(virtual_ptr != 0);
    CALL(string)->free(cvm, virtual_ptr);
#ifndef USE_GC
    CALL(user)->free(cvm, virtual_ptr);
#endif
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read__address, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const_pointer_ptr tmp;
    u64 virtual_ptr = CALL(virtual)->alloc(cvm, 1, TYPE_DATA);
    u64 address = CALL(options)->size + 1;
    const_pointer_ptr ref = CALL(virtual)->read(cvm, address);
    RX_ASSERT(virtual_ptr != 0);
    RX_ASSERT(ref == 0);
    CALL(pointer)->free(cvm, address);
    CALL(pointer)->free(cvm, virtual_ptr);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_type, .fixture = test_fixture_pointer) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const_pointer_ptr tmp;
    u64 virtual_ptr = CALL(virtual)->alloc(cvm, 1, TYPE_DATA);
    u64 address = CALL(options)->size + 1;
    u64 type_id = CALL(virtual)->type(cvm, address);
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
    CALL(virtual)->free(cvm, 0);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_free_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    CALL(virtual)->free(cvm, 1);
    RX_ASSERT(0 != 1);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 type_id = CALL(virtual)->type(cvm, 1);
    RX_ASSERT(type_id == TYPE_NULL);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_read_type_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    const_pointer_ptr ptr = CALL(virtual)->read(cvm, 0);
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
    const_pointer_ptr ptr = CALL(virtual)->read(cvm, 1);
    RX_ASSERT(ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_write_0, .fixture = test_fixture) {
    TEST_DATA rx = (TEST_DATA)RX_DATA;
    const_vm_ptr cvm = rx->ctx;
    u64 ptr = CALL(virtual)->alloc(cvm, 0, 0);
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
    CALL(vm)->dump(cvm);
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
RX_TEST_CASE(tests_vm_v1, test_vm_dump, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 text_string_ptr[] = {
        CALL(string)->load(cvm, "a\nb"),
        CALL(string)->load(cvm, "abc\nabcd\nbcde\nabc\na\nb"),
        CALL(string)->load(cvm, "ab\nabc\n")
    };
    parse_text(cvm, text_string_ptr[1]);
    CALL(vm)->dump(cvm);
    const_vm_ptr debug_cvm = CALL(vm)->init(8);
    u64 text_size = CALL(string)->size(cvm, text_string_ptr[1]);
    const_void_ptr data = CALL(string)->unsafe(cvm, text_string_ptr[1]);
    u64 debug_text_string_ptr = CALL(pointer)->copy(debug_cvm, data, text_size + 1, 0, TYPE_STRING);
    parse_text(debug_cvm, debug_text_string_ptr);
    CALL(vm)->dump(debug_cvm);
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
    stack_ptr stack = 0;
    CALL(list)->init(&stack);
    CALL(vm)->dump_stack(cvm, &stack);
    const_vm_ptr debug_cvm = CALL(vm)->init(8);
    u64 text_size = CALL(string)->size(cvm, text_string_ptr[1]);
    const_void_ptr data = CALL(string)->unsafe(cvm, text_string_ptr[1]);
    u64 debug_text_string_ptr = CALL(pointer)->copy(debug_cvm, data, text_size + 1, 0, TYPE_STRING);
    parse_text(debug_cvm, debug_text_string_ptr);
    stack_ptr debug_stack = 0;
    CALL(list)->init(&debug_stack);
    CALL(vm)->dump_stack(debug_cvm, &debug_stack);
#ifndef USE_GC
    for (u64 i = 0; i < sizeof(text_string_ptr) / sizeof(text_string_ptr[0]); i++) {
        CALL(string)->free(cvm, text_string_ptr[i]);
    }
    CALL(string)->free(debug_cvm, debug_text_string_ptr);
#else
    CALL(vm)->gc(debug_cvm);
    CALL(vm)->gc(cvm);
#endif
    CALL(list)->destroy(&debug_stack);
    CALL(list)->destroy(&stack);
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
    stack_ptr stack = 0;
    CALL(list)->init(&stack);
    CALL(vm)->dump_ref_stack(cvm, &stack);
    const_vm_ptr debug_cvm = CALL(vm)->init(8);
    u64 text_size = CALL(string)->size(cvm, text_string_ptr[1]);
    const_void_ptr data = CALL(string)->unsafe(cvm, text_string_ptr[1]);
    u64 debug_text_string_ptr = CALL(pointer)->copy(debug_cvm, data, text_size + 1, 0, TYPE_STRING);
    parse_text(debug_cvm, debug_text_string_ptr);
    stack_ptr debug_stack = 0;
    CALL(list)->init(&debug_stack);
    CALL(vm)->dump_ref_stack(debug_cvm, &debug_stack);
#ifndef USE_GC
    for (u64 i = 0; i < sizeof(text_string_ptr) / sizeof(text_string_ptr[0]); i++) {
        CALL(string)->free(cvm, text_string_ptr[i]);
    }
    CALL(string)->free(debug_cvm, debug_text_string_ptr);
#else
    CALL(vm)->gc(debug_cvm);
    CALL(vm)->gc(cvm);
#endif
    CALL(list)->destroy(&debug_stack);
    CALL(list)->destroy(&stack);
    CALL(vm)->destroy(debug_cvm);
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
    stack_ptr stack1 = 0;
    stack_ptr stack2 = 0;
    stack_ptr compare = 0;
    CALL(list)->init(&stack1);
    CALL(list)->init(&stack2);
    CALL(list)->init(&compare);
    u64 length = sizeof(src1) / sizeof(src1[0]);
    for (u64 i = 0; i < length; i++) {
        CALL(list)->push(&stack1, (void_ptr)src1[i]);
        CALL(list)->push(&stack2, (void_ptr)src2[i]);
    }
    CALL(list)->diff(&stack1, &stack2, &compare);
    stack_ptr current = compare;
    while (current->next != NULL_PTR) {
        u64 ptr = (u64)current->data;
        printf("A ^ B: %016llx\n", ptr);
        current = current->next;
    }
    CALL(list)->destroy(&stack1);
    CALL(list)->destroy(&stack2);
    CALL(list)->destroy(&compare);
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
    stack_ptr stack1 = 0;
    stack_ptr stack2 = 0;
    stack_ptr compare = 0;
    CALL(list)->init(&stack1);
    CALL(list)->init(&stack2);
    CALL(list)->init(&compare);
    u64 length = sizeof(src1) / sizeof(src1[0]);
    for (u64 i = 0; i < length; i++) {
        CALL(list)->push(&stack1, (void_ptr)src1[i]);
        CALL(list)->push(&stack2, (void_ptr)src2[i]);
    }
    CALL(list)->diff_left(&stack1, &stack2, &compare);
    stack_ptr current = compare;
    while (current->next != NULL_PTR) {
        u64 ptr = (u64)current->data;
        printf("A \\ B: %016llx\n", ptr);
        current = current->next;
    }
    CALL(list)->destroy(&stack1);
    CALL(list)->destroy(&stack2);
    CALL(list)->destroy(&compare);
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
    stack_ptr stack1 = 0;
    stack_ptr stack2 = 0;
    stack_ptr compare = 0;
    CALL(list)->init(&stack1);
    CALL(list)->init(&stack2);
    CALL(list)->init(&compare);
    u64 length = sizeof(src1) / sizeof(src1[0]);
    for (u64 i = 0; i < length; i++) {
        CALL(list)->push(&stack1, (void_ptr)src1[i]);
        CALL(list)->push(&stack2, (void_ptr)src2[i]);
    }
    CALL(list)->diff_right(&stack1, &stack2, &compare);
    stack_ptr current = compare;
    while (current->next != NULL_PTR) {
        u64 ptr = (u64)current->data;
        printf("B \\ A: %016llx\n", ptr);
        current = current->next;
    }
    CALL(list)->destroy(&stack1);
    CALL(list)->destroy(&stack2);
    CALL(list)->destroy(&compare);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_diff_0_0_0) {
    stack_ptr stack = 0;
    CALL(list)->diff(&stack, 0, 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_diff_right_0_0_0) {
    stack_ptr stack = 0;
    CALL(list)->diff_right(&stack, 0, 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_diff_left_0_0_0) {
    stack_ptr stack = 0;
    CALL(list)->diff_left(&stack, 0, 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_diff_0_0) {
    stack_ptr stack = 0;
    CALL(list)->init(&stack);
    CALL(list)->diff(&stack, 0, 0);
    CALL(list)->destroy(&stack);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_diff_right_0_0) {
    stack_ptr stack = 0;
    CALL(list)->init(&stack);
    CALL(list)->diff_right(&stack, 0, 0);
    CALL(list)->destroy(&stack);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_diff_left_0_0) {
    stack_ptr stack = 0;
    CALL(list)->init(&stack);
    CALL(list)->diff_left(&stack, 0, 0);
    CALL(list)->destroy(&stack);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_diff_0) {
    stack_ptr stack1 = 0;
    stack_ptr stack2 = 0;
    CALL(list)->init(&stack1);
    CALL(list)->init(&stack2);
    CALL(list)->diff(&stack1, 0, 0);
    CALL(list)->destroy(&stack1);
    CALL(list)->destroy(&stack2);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_diff_right_0) {
    stack_ptr stack1 = 0;
    stack_ptr stack2 = 0;
    CALL(list)->init(&stack1);
    CALL(list)->init(&stack2);
    CALL(list)->diff_right(&stack1, 0, 0);
    CALL(list)->destroy(&stack1);
    CALL(list)->destroy(&stack2);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_list_diff_left_0) {
    stack_ptr stack1 = 0;
    stack_ptr stack2 = 0;
    CALL(list)->init(&stack1);
    CALL(list)->init(&stack2);
    CALL(list)->diff_left(&stack1, 0, 0);
    CALL(list)->destroy(&stack1);
    CALL(list)->destroy(&stack2);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_dump_memory_leak_2, .fixture = test_clean_fixture) {
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

        CALL(vm)->dump(debug_cvm);
        stack_ptr debug_cvm_stack = 0;
        CALL(list)->init(&debug_cvm_stack);
        CALL(vm)->dump_ref_stack(debug_cvm, &debug_cvm_stack);

        u64 text_string_ptr = CALL(string)->load(cvm, test_data[i]);
        parse_text(cvm, text_string_ptr);
        CALL(string)->free(cvm, text_string_ptr);

        CALL(vm)->dump(cvm);
        stack_ptr cvm_stack = 0;
        CALL(list)->init(&cvm_stack);
        CALL(vm)->dump_ref_stack(cvm, &cvm_stack);

        // get unmatched pointers (it is possible to got memory leaks in both A and B memory dumps)
        // in this particual scenario we can safely assume memory leaks are in (A \ B) set

        stack_ptr compare_left = 0;
        CALL(list)->init(&compare_left);
        CALL(list)->diff_left(&debug_cvm_stack, &cvm_stack, &compare_left);
        stack_ptr current = 0;
        current = compare_left;
        while (current->next != NULL_PTR) {
            u64 ptr = (u64)current->data;
            printf("  v<: %016llx\n", ptr);
            const_pointer_ptr const_ptr = (const_pointer_ptr)ptr;
            CALL(pointer)->free(debug_cvm, const_ptr->public.address);
            current = current->next;
        }
        CALL(list)->destroy(&debug_cvm_stack);
        CALL(list)->destroy(&cvm_stack);
        CALL(list)->destroy(&compare_left);
    }
#ifdef USE_GC
    CALL(vm)->gc(debug_cvm);
    CALL(vm)->gc(cvm);
#endif
    CALL(vm)->destroy(debug_cvm);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_dump_stack_0, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    stack_ptr stack = 0;
    CALL(list)->init(&stack);
    CALL(vm)->dump_stack(cvm, &stack);
    void_ptr current = CALL(list)->peek(&stack);
    RX_ASSERT(current == 0);
#ifndef USE_GC
#else
    CALL(vm)->gc(cvm);
#endif
    CALL(list)->destroy(&stack);
    CALL(vm)->destroy(cvm);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_vm_dump_ref_stack_0, .fixture = test_clean_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    stack_ptr stack = 0;
    CALL(list)->init(&stack);
    CALL(vm)->dump_ref_stack(cvm, &stack);
    void_ptr current = CALL(list)->peek(&stack);
    RX_ASSERT(current == 0);
#ifndef USE_GC
#else
    CALL(vm)->gc(cvm);
#endif
    CALL(list)->destroy(&stack);
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
    u64 ptr_id = CALL(virtual)->alloc(cvm, 8, TYPE_USER + 1);
    const_pointer_ptr data_ptr = CALL(virtual)->read(cvm, ptr_id);
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
    u64 ptr_id = CALL(virtual)->alloc(cvm, 8, TYPE_DATA);
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
    u64 ptr_id = CALL(virtual)->alloc(cvm, 8, TYPE_DATA);
    CALL(pointer)->free(cvm, ptr_id);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
    const_pointer_ptr data_ptr = CALL(virtual)->read(cvm, ptr_id);
    RX_ASSERT(data_ptr == 0);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_gc_destroy_read_type_release_0, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(virtual)->alloc(cvm, 8, TYPE_DATA);
#ifndef USE_GC
    CALL(pointer)->free(cvm, ptr_id);
#endif
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
    const_pointer_ptr data_ptr = CALL(virtual)->read(cvm, ptr_id);
    RX_ASSERT(data_ptr == 0);
    CALL(pointer)->free(cvm, ptr_id);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_destroy_read_type_release_0, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(virtual)->alloc(cvm, 8, TYPE_DATA);
    CALL(vm)->release(cvm, ptr_id);
#ifndef USE_GC
    CALL(pointer)->free(cvm, ptr_id);
#endif
    CALL(vm)->destroy(cvm);
    const_pointer_ptr data_ptr = CALL(virtual)->read(cvm, ptr_id);
    RX_ASSERT(data_ptr == 0);
    CALL(pointer)->free(cvm, ptr_id);
}

/* test init */
RX_TEST_CASE(tests_vm_v1, test_pointer_alloc_alloc_ref_write_gc_destroy_read_type_release_0, .fixture = test_pointer_fixture) {
    const_vm_ptr cvm = CALL(vm)->init(8);
    u64 ptr_id = CALL(virtual)->alloc(cvm, 8, TYPE_DATA);
#ifndef USE_GC
    CALL(pointer)->free(cvm, ptr_id);
#endif
    CALL(vm)->release(cvm, ptr_id);
    CALL(vm)->gc(cvm);
    CALL(vm)->destroy(cvm);
    const_pointer_ptr data_ptr = CALL(virtual)->read(cvm, ptr_id);
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
    u64 stack_ptr2 = CALL(stack)->alloc(cvm); // this is leaking MP
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
            CALL(string_pointer)->free(cvm, string_ptr);
            continue;
        }
        CALL(env)->puts(cvm, string_ptr);
        u64 pattern_ptr = CALL(stack)->pop(cvm, stack_ptr2);
        if (CALL(string)->size(cvm, pattern_ptr) == 0) {
            quit = 1;
            CALL(string)->free(cvm, string_ptr);
            CALL(string_pointer)->free(cvm, string_ptr);
            CALL(string)->free(cvm, pattern_ptr);
            CALL(string_pointer)->free(cvm, pattern_ptr);
            continue;
        }
        CALL(env)->puts(cvm, pattern_ptr);
        u64 size = CALL(string)->size(cvm, pattern_ptr);
        u64 string_pointer_ptr = 0;
        u64 current_ptr = string_ptr;
        while ((string_pointer_ptr = CALL(string)->strchr(cvm, current_ptr, pattern_ptr)) != 0) {
            u64 match_ptr = CALL(string)->match(cvm, string_pointer_ptr, pattern_ptr);
            if (match_ptr == 0) {
                CALL(string)->free(cvm, string_pointer_ptr);
                CALL(string_pointer)->free(cvm, string_pointer_ptr);
                CALL(string)->free(cvm, string_ptr);
                CALL(string_pointer)->free(cvm, string_ptr);
                CALL(string)->free(cvm, pattern_ptr);
                CALL(string_pointer)->free(cvm, pattern_ptr);
                break;
            }
            if (CALL(string)->lessthan(cvm, string_pointer_ptr, match_ptr)) {
                u64 match_start_ptr = CALL(string)->left(cvm, match_ptr, size);
                u64 str_ncpy = CALL(string)->strncpy(cvm, match_start_ptr, size);
                u64 distance = CALL(string)->lessthan(cvm, string_ptr, match_start_ptr);
                if (distance > 0) {
                    u64 i = 0;
                    while (i++ < distance) {
                        printf(" ");
                    }
                }
                printf("%s[%lld]\n", CALL(string)->unsafe(cvm, str_ncpy), distance);
                CALL(string_pointer)->free(cvm, match_start_ptr);
                CALL(string)->free(cvm, str_ncpy);
            }
            CALL(string)->free(cvm, string_pointer_ptr);
            CALL(string_pointer)->free(cvm, string_pointer_ptr);
            current_ptr = match_ptr;
        }
#ifndef USE_GC
        CALL(string)->free(cvm, string_ptr);
        CALL(string_pointer)->free(cvm, string_ptr);
        CALL(string)->free(cvm, pattern_ptr);
        CALL(string_pointer)->free(cvm, pattern_ptr);
        CALL(string)->free(cvm, current_ptr);
        CALL(string_pointer)->free(cvm, current_ptr);
#endif
    }
#ifndef USE_GC
    CALL(stack)->free(cvm, stack_ptr2);
#endif
    CALL(stack)->free(cvm, stack_ptr1);
    CALL(stack)->free(cvm, stack_ptr2);
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
    u64 stack_ptr2 = CALL(stack)->alloc(cvm); // this is leaking MP
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
            CALL(string_pointer)->free(cvm, string_ptr);
            continue;
        }
        CALL(env)->puts(cvm, string_ptr);
        u64 pattern_ptr = CALL(stack)->pop(cvm, stack_ptr2);
        if (CALL(string)->size(cvm, pattern_ptr) == 0) {
            quit = 1;
            CALL(string)->free(cvm, string_ptr);
            CALL(string_pointer)->free(cvm, string_ptr);
            CALL(string)->free(cvm, pattern_ptr);
            CALL(string_pointer)->free(cvm, pattern_ptr);
            continue;
        }
        CALL(env)->puts(cvm, pattern_ptr);
        u64 size = CALL(string)->size(cvm, pattern_ptr);
        u64 string_pointer_ptr = 0;
        u64 match_ptr = 0;
        u64 current_ptr = string_ptr;
        u64 current_string_pointer_ptr = 0;
        while ((current_string_pointer_ptr = CALL(string)->strchr(cvm, current_ptr, pattern_ptr)) != 0) {
            string_pointer_ptr = current_string_pointer_ptr;
            match_ptr = CALL(string)->match(cvm, current_string_pointer_ptr, pattern_ptr);
            if (match_ptr == 0) {
                // direct memory leak: skips pointer free for string_pointer_ptr variable
                // CALL(list)->push(memory_leak_stack, (void_ptr)string_pointer_ptr);
                // TODO: UNSET MEMORY LEAK HERE
                // CALL(string)->free(cvm, string_pointer_ptr);
                // CALL(string_pointer)->free(cvm, string_pointer_ptr);
                CALL(string)->free(cvm, string_ptr);
                CALL(string_pointer)->free(cvm, string_ptr);
                CALL(string)->free(cvm, pattern_ptr);
                CALL(string_pointer)->free(cvm, pattern_ptr);
                break;
            } else {
                // indirect memory leak: overwrites pointer address for match_ptr variable
                // CALL(list)->push(memory_leak_stack, (void_ptr)match_ptr);
            }
            if (CALL(string)->lessthan(cvm, string_pointer_ptr, match_ptr)) {
                u64 match_start_ptr = CALL(string)->left(cvm, match_ptr, size);
                u64 str_ncpy = CALL(string)->strncpy(cvm, match_start_ptr, size);
                u64 distance = CALL(string)->lessthan(cvm, string_ptr, match_start_ptr);
                if (distance > 0) {
                    u64 i = 0;
                    while (i++ < distance) {
                        printf(" ");
                    }
                }
                printf("%s[%lld]\n", CALL(string)->unsafe(cvm, str_ncpy), distance);
                CALL(string_pointer)->free(cvm, match_start_ptr);
                CALL(string)->free(cvm, str_ncpy);
            }
            CALL(string)->free(cvm, string_pointer_ptr);
            CALL(string_pointer)->free(cvm, string_pointer_ptr);
            current_ptr = match_ptr;
        }
        if (match_ptr != 0 && current_ptr != match_ptr) {
            // indirect memory leak: overwrites pointer address for match_ptr variable
            // CALL(list)->push(memory_leak_stack, (void_ptr)match_ptr);
        }
#ifndef USE_GC
        CALL(string)->free(cvm, string_ptr);
        CALL(string_pointer)->free(cvm, string_ptr);
        CALL(string)->free(cvm, pattern_ptr);
        CALL(string_pointer)->free(cvm, pattern_ptr);
        CALL(string)->free(cvm, current_ptr);
        CALL(string_pointer)->free(cvm, current_ptr);
#endif
    }
#ifndef USE_GC
    CALL(stack)->free(cvm, stack_ptr2);
#endif
    CALL(stack)->free(cvm, stack_ptr1);
    CALL(stack)->free(cvm, stack_ptr2);
    CALL(stack)->free(cvm, gc_ptr);
}

const tests_vm_test_suite PRIVATE_API(tests_vm_test_suite_definitions) = {
    .run = run
};
