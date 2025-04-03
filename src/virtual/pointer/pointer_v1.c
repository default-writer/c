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

#include "std/data.h"

#include "pointer_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#define VM_TYPE_SIZE sizeof(vm_type)

#include "system/list/list_v1.h"
#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#ifdef USE_MEMORY_DEBUG_INFO
#include "virtual/pointer/pointer_v1.h"
#endif

#include "virtual/virtual/virtual_v1.h"

/* macros */
#define DEFAULT_SIZE 0x8 /* 8 */
#define POINTER_TYPE_SIZE sizeof(pointer_type)
#define KNOWN_TYPES_TYPE_SIZE sizeof(known_types_type)
#define KNOWN_TYPES_TYPE_ARRAY_SIZE(size) (size * KNOWN_TYPES_TYPE_SIZE)
#define POINTER_TYPE_SIZE sizeof(pointer_type)

/* internal */
#include "internal/internal_v1.h"
#include "internal/private_v1.h"

/* private */
typedef struct vm_state {
    virtual_pointer_ptr vptr;
    pointer_ptr* ref;
} vm_state_type;

#ifdef USE_MEMORY_DEBUG_INFO
static void pointer_dump(pointer_ptr ptr);
static void pointer_dump_ref(pointer_ptr* ptr);
#endif

/* internal */
struct file_handler {
    FILE* file;
#ifdef USE_MEMORY_DEBUG_INFO
    char* path;
#endif
};

struct list_handler {
    stack_ptr list;
};

/* public */
static void pointer_register_known_type(const_vm_ptr cvm, type_methods_definitions_ptr data_type);
static void pointer_register_user_type(const_vm_ptr cvm, type_methods_definitions_ptr data_type);
static u64 pointer_copy(const_vm_ptr cvm, const_void_ptr data, u64 size, u64 offset, u64 type_id);
static u64 pointer_alloc(const_vm_ptr cvm, const_void_ptr data, u64 size, u64 type_id);
static const_void_ptr pointer_read(const_vm_ptr cvm, u64 address, u64 type_id);
static u64 pointer_free(const_vm_ptr cvm, u64 address, u64 type_id);

/* internal */
static u64 pointer_alloc_internal(const_vm_ptr cvm, const_void_ptr data, const_pointer_ptr* tmp, u64 size, u64 type_id);
static void known_types_init_internal(const_vm_ptr cvm, type_methods_definitions_ptr data_type);
static void user_types_init_internal(const_vm_ptr cvm, type_methods_definitions_ptr data_type);
static u64 find_type_id_internal(const_vm_ptr cvm, const_type_methods_definitions_ptr data_type);

static void pointer_register_known_type(const_vm_ptr cvm, type_methods_definitions_ptr data_type) {
    known_types_init_internal(cvm, data_type);
}

static void pointer_register_user_type(const_vm_ptr cvm, type_methods_definitions_ptr data_type) {
    user_types_init_internal(cvm, data_type);
}

/* internal */
static u64 pointer_alloc_internal(const_vm_ptr cvm, const_void_ptr data, const_pointer_ptr* tmp, u64 size, u64 type_id) {
    u64 address = CALL(virtual)->alloc(cvm, size, type_id);
    *tmp = CALL(virtual)->read(cvm, address, type_id);
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = *tmp;
    pointer_ptr ptr = safe_ptr.ptr;
    ptr->data = data;
    return address;
}

static void known_types_init_internal(const_vm_ptr cvm, type_methods_definitions_ptr data_type) {
    known_types_ptr obj = CALL(os)->calloc(1, KNOWN_TYPES_TYPE_SIZE);
    obj->next = (*cvm)->known_types;
    obj->methods = data_type;
    (*cvm)->known_types = obj;
    (*cvm)->known_types_counter++;
}

static void user_types_init_internal(const_vm_ptr cvm, type_methods_definitions_ptr data_type) {
    u64 type_id = 0;
    if ((type_id = find_type_id_internal(cvm, data_type)) != 0) {
        ERROR_INVALID_TYPE_ID("type_id == %lld", type_id);
        return;
    }
    known_types_init_internal(cvm, data_type);
    data_type->type_id = (*cvm)->known_types_counter;
}

static u64 find_type_id_internal(const_vm_ptr cvm, const_type_methods_definitions_ptr data_type) {
    known_types_ptr current = (*cvm)->known_types;
    while (current != 0) {
        known_types_ptr prev = current->next;
        if (current->methods == data_type) {
            return data_type->type_id;
        }
        current = prev;
    }
    return 0;
}

/* public */
static u64 pointer_alloc(const_vm_ptr cvm, const_void_ptr data, u64 size, u64 type_id) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (data == 0) {
        ERROR_INVALID_ARGUMENT("data == %p", data);
        return FALSE;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT("size == %lld", size);
        return FALSE;
    }
    if (type_id == 0) {
        ERROR_INVALID_ARGUMENT("type_id == %lld", type_id);
        return FALSE;
    }
    const_pointer_ptr tmp;
    u64 address = pointer_alloc_internal(cvm, data, &tmp, size, type_id);
    return address;
}

static u64 pointer_copy(const_vm_ptr cvm, const_void_ptr data, u64 size, u64 offset, u64 type_id) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (data == 0) {
        ERROR_INVALID_ARGUMENT("data == %p", data);
        return FALSE;
    }
    if (size == 0) {
        ERROR_INVALID_ARGUMENT("size == %lld", size);
        return FALSE;
    }
    if (type_id == 0) {
        ERROR_INVALID_ARGUMENT("type_id == %lld", type_id);
        return FALSE;
    }
    void_ptr new_data = CALL(memory)->alloc(size);
    const_pointer_ptr tmp;
    u64 address = pointer_alloc_internal(cvm, new_data, &tmp, size, type_id);
    CALL(os)->memcpy(new_data, data, size);
    u64 data_size = size;
    if (offset > 0 && offset < data_size) {
        safe_void_ptr safe_ptr;
        safe_ptr.const_ptr = tmp->data;
        pointer_ptr ptr = safe_ptr.ptr;
        void_ptr data_ptr = ptr;
        if (data_ptr != 0) {
            u8* bytes = data_ptr;
            bytes[offset] = 0;
        }
    }
    return address;
}

static const_void_ptr pointer_read(const_vm_ptr cvm, u64 address, u64 type_id) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return NULL_PTR;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return NULL_PTR;
    }
    if (type_id == 0) {
        ERROR_INVALID_ARGUMENT("type_id == %lld", type_id);
        return NULL_PTR;
    }
    const_pointer_ptr const_ptr = CALL(virtual)->read(cvm, address, type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, type_id);
        return NULL_PTR;
    }
    const_void_ptr data = const_ptr->data;
    return data;
}

static u64 pointer_free(const_vm_ptr cvm, u64 address, u64 type_id) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    if (type_id == 0) {
        ERROR_INVALID_ARGUMENT("type_id == %lld", type_id);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(virtual)->read(cvm, address, type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, type_id);
        return FALSE;
    }
    safe_pointer_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr;
    pointer_ptr ptr = safe_ptr.ptr;
    u64 data_size = const_ptr->public.size;
    if (data_size != 0) {
        CALL(memory)->free(ptr->data, data_size);
    }
    CALL(virtual)->free(cvm, address);
    CALL(os)->memset(ptr, 0, POINTER_TYPE_SIZE); /* NOLINT: sizeof(pointer_type) */
    CALL(os)->free(ptr);
    return TRUE;
}

/* code */

/* public */
const virtual_pointer_methods PRIVATE_API(virtual_pointer_methods_definitions) = {
    .alloc = pointer_alloc,
    .copy = pointer_copy,
    .read = pointer_read,
    .free = pointer_free
};

const virtual_pointer_methods* PRIVATE_API(pointer) = &PRIVATE_API(virtual_pointer_methods_definitions);
const virtual_pointer_methods* CALL(pointer) {
    return PRIVATE_API(pointer);
}

const virtual_type_methods PRIVATE_API(virtual_type_methods_definitions) = {
    .register_known_type = pointer_register_known_type,
    .register_user_type = pointer_register_user_type
};

const virtual_type_methods* PRIVATE_API(type) = &PRIVATE_API(virtual_type_methods_definitions);
const virtual_type_methods* CALL(type) {
    return PRIVATE_API(type);
}
