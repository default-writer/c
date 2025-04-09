/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 9, 2025 at 3:59:59 PM GMT+3
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

/* internal */
static u64 stack_alloc_internal(const_vm_ptr cvm);
static void stack_release_internal(const_vm_ptr cvm, stack_ptr curent);

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

/* definition */
typedef struct stack_handler* stack_handler_ptr;
typedef const struct stack_handler* const_stack_handler_ptr;
typedef struct stack_handler {
    u64 size;
    stack_ptr list;
} stack_handler_type;

/* destructor */
static void stack_type_destructor(const_vm_ptr cvm, u64 address);

/* implementation */
static struct type_methods_definitions stack_type_definitions = {
    .type_id = TYPE_STACK,
    .destructor = stack_type_destructor
};

static void stack_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)stack_type_definitions.type_id);
        return;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr handler = data_ptr;
    handler->size = 0;
    stack_release_internal(cvm, handler->list);
    CALL(list)->destroy(cvm, handler->list);
    CALL(pointer)->free(cvm, address);
}

/* internal */
static u64 stack_alloc_internal(const_vm_ptr cvm) {
    void_ptr data = CALL(memory)->alloc(STACK_HANDLER_TYPE_SIZE);
    u64 ptr = CALL(pointer)->alloc(cvm, data, STACK_HANDLER_TYPE_SIZE, stack_type_definitions.type_id);
    stack_handler_ptr handler = data;
    handler->size = 0;
    handler->list = 0;
    return ptr;
}

static void stack_release_internal(const_vm_ptr cvm, stack_ptr current) {
    u64 ptr = 0;
    while ((ptr = (u64)CALL(list)->pop(cvm, current)) != 0) {
        CALL(vm)->release(cvm, ptr);
    }
}

/* public */
static u64 stack_alloc(const_vm_ptr cvm) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    u64 ptr = stack_alloc_internal(cvm);
    return ptr;
}

static u64 stack_release(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)stack_type_definitions.type_id);
        return FALSE;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr handler = data_ptr;
    handler->size = 0;
    stack_release_internal(cvm, handler->list);
    return TRUE;
}

static u64 stack_push(const_vm_ptr cvm, u64 ptr_list, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (ptr_list == 0) {
        ERROR_INVALID_ARGUMENT("ptr_list = %lld", ptr_list);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    if (ptr_list == address) {
        ERROR_INVALID_VALUE("ptr_list = %lld, address = %lld", ptr_list, address);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, ptr_list, stack_type_definitions.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, ptr_list, (u64)stack_type_definitions.type_id);
        return FALSE;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr handler = data_ptr;
    if (handler->list == 0) {
        handler->list = CALL(list)->init(cvm);
    }
    CALL(list)->push(cvm, handler->list, (void_ptr)address);
    handler->size++;
    return TRUE;
}

static u64 stack_peek(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address = %lld", address);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_VALUE("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)stack_type_definitions.type_id);
        return FALSE;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr handler = data_ptr;
    u64 stack_peek_ptr = (u64)CALL(list)->peek(cvm, handler->list);
    return stack_peek_ptr;
}

static u64 stack_peekn(const_vm_ptr cvm, u64 address, u64 nelements) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address = %lld", address);
        return FALSE;
    }
    if (nelements == 0) {
        ERROR_INVALID_ARGUMENT("nelements = %lld", nelements);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_VALUE("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)stack_type_definitions.type_id);
        return FALSE;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr src_handler = data_ptr;
    u64 size = src_handler->size;
    if (size == 0) {
        ERROR_INVALID_VALUE("size == %lld", size);
        return FALSE;
    }
    if (size < nelements) {
        ERROR_INVALID_VALUE("size == %lld, nelements == %lld", size, nelements);
        return FALSE;
    }
    u64 dst = stack_alloc_internal(cvm);
    const_pointer_ptr dst_const_ptr = CALL(pointer)->read(cvm, dst, stack_type_definitions.type_id);
    safe_void_ptr void_dst_ptr;
    void_dst_ptr.const_ptr = dst_const_ptr->data;
    void_ptr dst_ptr = void_dst_ptr.ptr;
    stack_handler_ptr dst_handler = dst_ptr;
    u64 i = nelements;
    stack_ptr src_handler_list = src_handler->list;
    stack_element_ptr current = src_handler_list->current;
    if (dst_handler->list == 0) {
        dst_handler->list = CALL(list)->init(cvm);
    }
    while (i-- > 0) {
        CALL(list)->push(cvm, dst_handler->list, (void_ptr)current->data);
        current = current->next;
        dst_handler->size++;
    }
    return dst;
}

static u64 stack_pop(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address = %lld", address);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)stack_type_definitions.type_id);
        return FALSE;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr handler = data_ptr;
    if (handler->size == 0) {
        return FALSE;
    }
    u64 stack_pop_ptr = (u64)CALL(list)->pop(cvm, handler->list);
    handler->size--;
    return stack_pop_ptr;
}

static u64 stack_popn(const_vm_ptr cvm, u64 address, u64 nelements) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address = %lld", address);
        return FALSE;
    }
    if (nelements == 0) {
        ERROR_INVALID_ARGUMENT("nelements = %lld", nelements);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr = %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)stack_type_definitions.type_id);
        return FALSE;
    }
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = const_ptr->data;
    void_ptr src_data_ptr = src_void_ptr.ptr;
    stack_handler_ptr src_handler = src_data_ptr;
    u64 size = src_handler->size;
    if (size == 0) {
        ERROR_INVALID_VALUE("size == %lld", size);
        return FALSE;
    }
    if (size < nelements) {
        ERROR_INVALID_VALUE("size == %lld, nelements == %lld", size, nelements);
        return FALSE;
    }
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
        dst_handler->size++;
    }
    return dst;
}

static u64 stack_size(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address = %lld", address);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type_definitions.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)stack_type_definitions.type_id);
        return FALSE;
    }
    const_stack_handler_ptr handler = const_ptr->data;
    u64 size = handler->size;
    return size;
}

static u64 stack_free(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
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
