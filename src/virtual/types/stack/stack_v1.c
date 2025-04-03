/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 3, 2025 at 11:20:29 AM GMT+3
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

#include "stack_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/list/list_v1.h"
#include "system/memory/memory_v1.h"

#include "virtual/pointer/pointer_v1.h"
#include "virtual/vm/vm_v1.h"

#define STACK_HANDLER_TYPE_SIZE sizeof(stack_handler_type)

/* internal */
static u64 stack_alloc_internal(const_vm_ptr cvm);
static void stack_release_internal(const_vm_ptr cvm, stack_ptr* curent);

/* public */
static u64 stack_alloc(const_vm_ptr cvm);
static u64 stack_free(const_vm_ptr cvm, u64 ptr);
static u64 stack_push(const_vm_ptr cvm, u64 ptr, u64 data_ptr);
static u64 stack_peek(const_vm_ptr cvm, u64 ptr);
static u64 stack_peekn(const_vm_ptr cvm, u64 ptr, u64 nelements);
static u64 stack_pop(const_vm_ptr cvm, u64 ptr);
static u64 stack_popn(const_vm_ptr cvm, u64 ptr, u64 nelements);
static u64 stack_size(const_vm_ptr cvm, u64 ptr);
static u64 stack_release(const_vm_ptr cvm, u64 ptr);

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
static struct type_methods_definitions stack_type = {
    .type_id = TYPE_STACK,
    .destructor = stack_type_destructor
};

static void stack_type_destructor(const_vm_ptr cvm, u64 address) {
    const_void_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)stack_type.type_id);
        return;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr handler = data_ptr;
    handler->size = 0;
    stack_release_internal(cvm, &handler->list);
    CALL(list)->destroy(&handler->list);
    CALL(pointer)->free(cvm, address, stack_type.type_id);
}

/* internal */
static u64 stack_alloc_internal(const_vm_ptr cvm) {
    void_ptr data = CALL(memory)->alloc(STACK_HANDLER_TYPE_SIZE);
    u64 ptr = CALL(pointer)->alloc(cvm, data, STACK_HANDLER_TYPE_SIZE, stack_type.type_id);
    stack_handler_ptr handler = data;
    handler->size = 0;
    CALL(list)->init(&handler->list);
    return ptr;
}

static void stack_release_internal(const_vm_ptr cvm, stack_ptr* current) {
    u64 ptr = 0;
    while ((ptr = (u64)CALL(list)->pop(current)) != 0) {
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
    const_void_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)stack_type.type_id);
        return FALSE;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr handler = data_ptr;
    handler->size = 0;
    stack_release_internal(cvm, &handler->list);
    return TRUE;
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
    const_void_ptr const_ptr = CALL(pointer)->read(cvm, ptr_list, stack_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, ptr_list == %lld, type_id == %lld", (const_void_ptr)const_ptr, ptr_list, (u64)stack_type.type_id);
        return FALSE;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr handler = data_ptr;
    CALL(list)->push(&handler->list, (void_ptr)address);
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
    const_void_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_VALUE("const_ptr == %p, address == %lld, type_id == %lld", const_ptr, address, (u64)stack_type.type_id);
        return FALSE;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    void_ptr data_ptr = safe_ptr.ptr;

    stack_handler_ptr handler = data_ptr;
    u64 stack_peek_ptr = (u64)CALL(list)->peek(&handler->list);
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
    const_void_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_VALUE("const_ptr == %p, address == %lld, type_id == %lld", const_ptr, address, (u64)stack_type.type_id);
        return FALSE;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
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
    const_void_ptr const_dst_ptr = CALL(pointer)->read(cvm, dst, stack_type.type_id);
    safe_void_ptr void_dst_ptr;
    void_dst_ptr.const_ptr = const_dst_ptr;
    void_ptr dst_ptr = void_dst_ptr.ptr;
    stack_handler_ptr dst_handler = dst_ptr;
    u64 i = nelements;
    while (i-- > 0) {
        stack_ptr current = src_handler->list;
        u64 stack_peek_ptr = (u64)CALL(list)->peek(&current);
        CALL(list)->push(&dst_handler->list, (void_ptr)stack_peek_ptr);
        dst_handler->size++;
        current = current->next;
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
    const_void_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)stack_type.type_id);
        return FALSE;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    void_ptr data_ptr = safe_ptr.ptr;
    stack_handler_ptr handler = data_ptr;
    if (handler->size == 0) {
        return FALSE;
    }
    u64 stack_pop_ptr = (u64)CALL(list)->pop(&handler->list);
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
    const_void_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr = %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)stack_type.type_id);
        return FALSE;
    }
    safe_void_ptr src_void_ptr;
    src_void_ptr.const_ptr = const_ptr;
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
    const_void_ptr dst_const_data_ptr = CALL(pointer)->read(cvm, dst, stack_type.type_id);
    safe_void_ptr dst_void_ptr;
    dst_void_ptr.const_ptr = dst_const_data_ptr;
    void_ptr dst_data_ptr = dst_void_ptr.ptr;
    stack_handler_ptr dst_handler = dst_data_ptr;
    u64 i = nelements;
    while (i-- > 0) {
        u64 stack_pop_ptr = (u64)CALL(list)->pop(&src_handler->list);
        CALL(list)->push(&dst_handler->list, (void_ptr)stack_pop_ptr);
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
    const_void_ptr const_ptr = CALL(pointer)->read(cvm, address, stack_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)stack_type.type_id);
        return FALSE;
    }
    const_stack_handler_ptr handler = const_ptr;
    u64 size = handler->size;
    return size;
}

/* public */
CVM_EXPORT void stack_init(const_vm_ptr cvm) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &stack_type;
    CALL(type)->register_known_type(cvm, safe_ptr.ptr);
}

const virtual_stack_methods PRIVATE_API(virtual_stack_methods_definitions) = {
    .alloc = stack_alloc,
    .free = stack_free,
    .push = stack_push,
    .peek = stack_peek,
    .peekn = stack_peekn,
    .pop = stack_pop,
    .popn = stack_popn,
    .size = stack_size,
    .release = stack_release
};

const virtual_stack_methods* PRIVATE_API(stack) = &PRIVATE_API(virtual_stack_methods_definitions);
const virtual_stack_methods* CALL(stack) {
    return PRIVATE_API(stack);
}
