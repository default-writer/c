/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 7, 2025 at 6:13:39 PM GMT+3
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

#include "file_v1.h"

#define USING_SYSTEM_ERROR_API
#include "system/error/error_v1.h"

#include "system/memory/memory_v1.h"
#include "system/os/os_v1.h"

#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/data/data_v1.h"

#include "internal/pointer_type_v1.h"

#define FILE_HANDLER_TYPE_SIZE sizeof(file_handler_type)

/* definition */
typedef struct file_handler* file_handler_ptr;
typedef struct file_handler {
    FILE* file;
#ifdef USE_MEMORY_DEBUG_INFO
    const char* path;
#endif
} file_handler_type;

/* public */
static u64 file_alloc(const_vm_ptr cvm, u64 file_path_ptr, u64 mode_ptr);
static u64 file_free(const_vm_ptr cvm, u64 address);
static u64 file_data(const_vm_ptr cvm, u64 address);

/* type */
static void file_type_destructor(const_vm_ptr cvm, u64 address);

/* implementation */
static struct type_methods_definitions file_type = {
    .type_id = TYPE_FILE,
    .destructor = file_type_destructor
};

static void file_type_destructor(const_vm_ptr cvm, u64 address) {
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, file_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)file_type.type_id);
        return;
    }
    CALL(pointer)->free(cvm, address);
}

static int is_valid_file_path(const char* file_path) {
    size_t size = CALL(os)->strlen(file_path);
    if (size > 0 && size <= PATH_MAX - 1) {
        return TRUE;
    }
    return FALSE;
}

static int is_valid_mode(const char* mode) {
    const char* valid_modes[] = { "r", "w", "a", "r+", "w+", "a+", "rb", "wb", "ab", "r+b", "w+b", "a+b" };
    size_t num_modes = sizeof(valid_modes) / sizeof(valid_modes[0]);
    for (size_t i = 0; i < num_modes; ++i) {
        if (CALL(os)->strcmp(mode, valid_modes[i]) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

static u64 file_alloc(const_vm_ptr cvm, u64 path, u64 mode) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (path == 0) {
        ERROR_INVALID_ARGUMENT("path == %lld", path);
        return FALSE;
    }
    if (mode == 0) {
        ERROR_INVALID_ARGUMENT("mode == %lld", mode);
        return FALSE;
    }
    const_pointer_ptr path_const_ptr = CALL(pointer)->read(cvm, path, TYPE_STRING);
    if (path_const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)path_const_ptr, path, (u64)TYPE_STRING);
        return FALSE;
    }
    const char* file_path_data = path_const_ptr->data;
    if (is_valid_file_path(file_path_data) == 0) {
        ERROR_INVALID_VALUE("file_path_data == %s", file_path_data);
        return FALSE;
    }
    const_pointer_ptr mode_pointer_ptr = CALL(pointer)->read(cvm, mode, TYPE_STRING);
    if (mode_pointer_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)mode_pointer_ptr, mode, (u64)TYPE_STRING);
        return FALSE;
    }
    const char* mode_data = mode_pointer_ptr->data;
    if (is_valid_mode(mode_data) == 0) {
        ERROR_INVALID_VALUE("mode_data == %s", mode_data);
        return FALSE;
    }
    FILE* f = CALL(os)->fopen(file_path_data, mode_data); /* NOLINT */
    if (f == 0) {
        ERROR_INVALID_VALUE("f == %p, file_path_data == %s, mode_data == %s", (void_ptr)f, file_path_data, mode_data);
        return FALSE;
    }
    void_ptr data = CALL(memory)->alloc(FILE_HANDLER_TYPE_SIZE);
    u64 address = CALL(pointer)->alloc(cvm, data, FILE_HANDLER_TYPE_SIZE, file_type.type_id);
    file_handler_ptr handler = data;
    handler->file = f;
#ifdef USE_MEMORY_DEBUG_INFO
    handler->path = file_path_data;
#endif
    return address;
}

static u64 file_data(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    const_pointer_ptr const_ptr = CALL(pointer)->read(cvm, address, file_type.type_id);
    if (const_ptr == 0) {
        ERROR_INVALID_POINTER("const_ptr == %p, address == %lld, type_id == %lld", (const_void_ptr)const_ptr, address, (u64)file_type.type_id);
        return FALSE;
    }
    safe_void_ptr safe_ptr;
    safe_ptr.const_ptr = const_ptr->data;
    void_ptr data_ptr = safe_ptr.ptr;
    file_handler_ptr handler = data_ptr;
    FILE* f = handler->file;
    if (CALL(os)->fseek(f, 0, SEEK_END) == 0) { /* NOLINT */
        long size = CALL(os)->ftell(f);
        if (size >= 0) {
            u64 data_size = (u64)size;
            u64 data_handle = CALL(data)->alloc(cvm, data_size + 1);
            void_ptr file_data = CALL(data)->unsafe(cvm, data_handle);
            if (CALL(os)->fseek(f, 0, SEEK_SET) == 0) { /* NOLINT */
                u64 read = CALL(os)->fread(file_data, 1, data_size, handler->file);
                if (handler->file != 0) {
                    CALL(os)->fclose(handler->file);
                    handler->file = 0;
                }
                if (read == data_size) {
                    return data_handle;
                }
            }
        }
    }
    return 0;
}

static u64 file_free(const_vm_ptr cvm, u64 address) {
    if (cvm == 0 || *cvm == 0) {
        ERROR_VM_NOT_INITIALIZED("cvm == %p", (const_void_ptr)cvm);
        return FALSE;
    }
    if (address == 0) {
        ERROR_INVALID_ARGUMENT("address == %lld", address);
        return FALSE;
    }
    file_type_destructor(cvm, address);
    return TRUE;
}

/* public */
CVM_EXPORT void file_init(const_vm_ptr cvm) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &file_type;
    CALL(type)->register_known_type(cvm, safe_ptr.ptr);
}

/*! definition (initialization) of file_methods structure */
const virtual_file_methods PRIVATE_API(virtual_file_methods_definitions) = {
    .alloc = file_alloc,
    .data = file_data,
    .free = file_free
};

const virtual_file_methods* PRIVATE_API(file) = &PRIVATE_API(virtual_file_methods_definitions);
const virtual_file_methods* CALL(file) {
    return PRIVATE_API(file);
}
