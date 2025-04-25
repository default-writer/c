/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 25, 2025 at 11:39:18 AM GMT+3
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

#include "stack_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/memory/memory_v1.h"

#include "virtual/list/list_v1.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/vm/vm_v1.h"

#include "internal/pointer_type_v1.h"

#define STACK_HANDLER_TYPE_SIZE sizeof(stack_handler_type)

/* definition */
typedef struct stack_handler* stack_handler_ptr;
typedef const struct stack_handler* const_stack_handler_ptr;
typedef struct stack_handler {
    stack_ptr list;
} stack_handler_type;

/* internal */
static u64 stack_alloc_internal(const_vm_ptr cvm);
static void stack_release_internal(const_vm_ptr cvm, stack_handler_ptr ptr);

/* public */
static u64 stack_alloc(const_vm_ptr cvm);
static u64 stack_push(const_vm_ptr cvm, u64 ptr, u64 data_ptr);
static u64 stack_peek(const_vm_ptr cvm, u64 ptr);
static u64 stack_peekn(const_vm_ptr cvm, u64 ptr, u64 nelements);
static u64 stack_pop(const_vm_ptr cvm, u64 ptr);
static u64 stack_popn(const_vm_ptr cvm, u64 ptr, u64 nelements);
static u64 stack_size(const_vm_ptr cvm, u64 ptr);
static u64 stack_release(const_vm_ptr cvm, u64 ptr);
static u64 stack_free(const_vm_ptr cvm, u64 ptr);

/* destructor */
static void stack_type_destructor(const_vm_ptr cvm, u64 address);

/* implementation */
static struct type_methods_definitions stack_type_definitions = {
    .type_id = TYPE_STACK,
    .destructor = stack_type_destructor
};

static void stack_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    CHECK_POINTER_VOID(const_ptr);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr handler = data_ptr;
    stack_release_internal(cvm, handler);
    CALL(list)->destroy(cvm, handler->list);
    CALL(pointer)->free(cvm, address);
}

/* internal */
static u64 stack_alloc_internal(const_vm_ptr cvm) {
    void_ptr data = CALL(memory)->alloc(STACK_HANDLER_TYPE_SIZE);
    u64 ptr = CALL(pointer)->alloc(cvm, data, STACK_HANDLER_TYPE_SIZE, 0, FLAG_MEMORY_PTR, stack_type_definitions.type_id);
    stack_handler_ptr handler = data;
    handler->list = CALL(list)->init(cvm);
    return ptr;
}

static void stack_release_internal(const_vm_ptr cvm, stack_handler_ptr handler) {
    while (handler->list->size > 0) {
        u64 ptr = (u64)CALL(list)->pop(cvm, handler->list);
        CALL(vm)->release(cvm, ptr);
    }
}

/* public */
static u64 stack_alloc(const_vm_ptr cvm) {
    CHECK_VM(cvm, FALSE);
    u64 ptr = stack_alloc_internal(cvm);
    return ptr;
}

static u64 stack_release(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(address, FALSE);
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    CHECK_POINTER(const_ptr, FALSE);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    u64 data_ptr;
    while ((data_ptr = (u64)stack_pop(cvm, address)) != 0) {
        CALL(pointer)->free(cvm, data_ptr);
    }
    stack_release_internal(cvm, safe_ptr.ptr);
    return TRUE;
}

static u64 stack_push(const_vm_ptr cvm, u64 ptr_list, u64 address) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(ptr_list, FALSE);
    CHECK_ARG(address, FALSE);
    CHECK_CONDITION(ptr_list == address, FALSE);
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, ptr_list, stack_type_definitions.type_id);
    CHECK_POINTER(const_ptr, FALSE);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr handler = data_ptr;
    CALL(list)->push(cvm, handler->list, (void_ptr)address);
    return TRUE;
}

static u64 stack_peek(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(address, NULL_ADDRESS);
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr handler = data_ptr;
    u64 stack_peek_ptr = (u64)CALL(list)->peek(cvm, handler->list);
    return stack_peek_ptr;
}

static u64 stack_peekn(const_vm_ptr cvm, u64 address, u64 nelements) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(address, NULL_ADDRESS);
    CHECK_ARG(nelements, NULL_ADDRESS);
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr src_handler = data_ptr;
    u64 size = src_handler->list->size;
    CHECK_VALUE(size, NULL_ADDRESS);
    CHECK_CONDITION(size < nelements, NULL_ADDRESS);
    u64 dst = stack_alloc_internal(cvm);
    const_pointer_ptr dst_const_ptr = CALL(pointer)->read(cvm, dst, stack_type_definitions.type_id);
    safe_void_ptr void_dst_ptr;
    void_dst_ptr.const_ptr = dst_const_ptr->data;
    void_ptr dst_ptr = void_dst_ptr.ptr;
    stack_handler_ptr dst_handler = dst_ptr;
    u64 i = nelements;
    stack_ptr src_handler_list = src_handler->list;
    stack_element_ptr current = src_handler_list->current;
    while (i-- > 0) {
        CALL(list)->push(cvm, dst_handler->list, (void_ptr)current->data);
        current = current->next;
    }
    return dst;
}

static u64 stack_pop(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(address, NULL_ADDRESS);
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr handler = data_ptr;
    if (handler->list->size == 0) {
        return NULL_ADDRESS;
    }
    u64 stack_pop_ptr = (u64)CALL(list)->pop(cvm, handler->list);
    return stack_pop_ptr;
}

static u64 stack_popn(const_vm_ptr cvm, u64 address, u64 nelements) {
    CHECK_VM(cvm, NULL_ADDRESS);
    CHECK_ARG(address, NULL_ADDRESS);
    CHECK_ARG(nelements, NULL_ADDRESS);
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    CHECK_POINTER(const_ptr, NULL_ADDRESS);
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = const_ptr->data;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    stack_handler_ptr src_handler = src_data_ptr;
    u64 size = src_handler->list->size;
    CHECK_VALUE(size, NULL_ADDRESS);
    CHECK_CONDITION(size < nelements, NULL_ADDRESS);
    u64 dst = stack_alloc_internal(cvm);
    const_pointer_ptr dst_const_ptr = CALL(pointer)->read(cvm, dst, stack_type_definitions.type_id);
    safe_void_ptr dst_void_ptr;
    dst_void_ptr.const_ptr = dst_const_ptr->data;
    void_ptr dst_data_ptr = dst_void_ptr.ptr;
    const stack_handler_ptr dst_handler = dst_data_ptr;
    u64 i = nelements;
    while (i-- > 0) {
        u64 stack_pop_ptr = (u64)CALL(list)->pop(cvm, src_handler->list);
        CALL(list)->push(cvm, dst_handler->list, (void_ptr)stack_pop_ptr);
    }
    return dst;
}

static u64 stack_size(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, NULL_VALUE);
    CHECK_ARG(address, NULL_ADDRESS);
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    CHECK_POINTER(const_ptr, NULL_VALUE);
    const_stack_handler_ptr handler = const_ptr->data;
    u64 size = handler->list->size;
    return size;
}

static u64 stack_free(const_vm_ptr cvm, u64 address) {
    CHECK_VM(cvm, FALSE);
    CHECK_ARG(address, FALSE);
    stack_type_destructor(cvm, address);
    return TRUE;
}

/* public */
CVM_EXPORT void stack_init(const_vm_ptr cvm) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &stack_type_definitions;
    CALL(type)->register_known_type(cvm, safe_ptr.ptr);
}

const virtual_stack_methods PRIVATE_API(virtual_stack_methods_definitions) = {
    .alloc = stack_alloc,
    .push = stack_push,
    .peek = stack_peek,
    .peekn = stack_peekn,
    .pop = stack_pop,
    .popn = stack_popn,
    .size = stack_size,
    .release = stack_release,
    .free = stack_free
};

const virtual_stack_methods* PRIVATE_API(stack) = &PRIVATE_API(virtual_stack_methods_definitions);
const virtual_stack_methods* CALL(stack) {
    return PRIVATE_API(stack);
}
