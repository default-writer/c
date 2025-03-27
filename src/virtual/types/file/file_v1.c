/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 27, 2025 at 4:39:26 PM GMT+3
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

#include "system/api/api_v1.h"
#include "system/memory/memory_v1.h"

#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/data/data_v1.h"

#define FILE_HANDLER_TYPE_SIZE sizeof(file_handler_type)

static const enum type type_id = TYPE_FILE;

/* definition */
typedef struct file_handler* file_handler_ptr;
typedef struct file_handler {
    FILE* file;
#ifdef USE_MEMORY_DEBUG_INFO
    const char* path;
#endif
} file_handler_type;

/* public */
static u64 file_alloc(const_vm_ptr vm, u64 file_path_ptr, u64 mode_ptr);
static u64 file_free(const_vm_ptr vm, u64 address);
static u64 file_data(const_vm_ptr vm, u64 address);

/* type */
static void file_type_destructor(u64 address);

/* implementation */
static struct type_methods_definitions file_type = {
    .destructor = file_type_destructor
};

static void INIT init(void) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &file_type;
    CALL(vm)->register_known_type(type_id, safe_ptr.ptr);
}

static void file_type_destructor(u64 address) {
    const_void_ptr const_data_ptr = CALL(pointer)->read(address, type_id);
    safe_void_ptr void_ptr;
    void_ptr.const_ptr = const_data_ptr;
    void* data_ptr = void_ptr.ptr;
    if (data_ptr == 0) {
        ERROR_INVALID_POINTER(data_ptr == 0);
        return;
    }
    file_handler_ptr handler = data_ptr;
    if (handler->file != 0) {
        api->fclose(handler->file);
        handler->file = 0;
    }
    CALL(pointer)->free(address, type_id);
}

static int is_valid_file_path(const char* file_path) {
    size_t size = api->strlen(file_path);
    if (size > 0 && size <= PATH_MAX - 1) {
        return TRUE;
    }
    return FALSE;
}

static int is_valid_mode(const char* mode) {
    const char* valid_modes[] = { "r", "w", "a", "r+", "w+", "a+", "rb", "wb", "ab", "r+b", "w+b", "a+b" };
    size_t num_modes = sizeof(valid_modes) / sizeof(valid_modes[0]);
    for (size_t i = 0; i < num_modes; ++i) {
        if (api->strcmp(mode, valid_modes[i]) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

static u64 file_alloc(const_vm_ptr vm, u64 file_path, u64 mode) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    if (file_path == 0) {
        ERROR_INVALID_ARGUMENT(file_path == 0);
        return FALSE;
    }
    if (mode == 0) {
        ERROR_INVALID_ARGUMENT(mode == 0);
        return FALSE;
    }
    const char* file_path_data = CALL(pointer)->read(file_path, TYPE_STRING);
    if (file_path_data == 0) {
        ERROR_INVALID_POINTER(mode_data == 0);
        return FALSE;
    }
    if (is_valid_file_path(file_path_data) == 0) {
        ERROR_INVALID_CONDITION(is_valid_file_path(file_path_data) == 0);
        return FALSE;
    }
    const char* mode_data = CALL(pointer)->read(mode, TYPE_STRING);
    if (mode_data == 0) {
        ERROR_INVALID_POINTER(mode_data == 0);
        return FALSE;
    }
    if (is_valid_mode(mode_data) == 0) {
        ERROR_INVALID_CONDITION(is_valid_mode(mode_data) == 0);
        return FALSE;
    }
    FILE* f = api->fopen(file_path_data, mode_data); /* NOLINT */
    if (f == 0) {
        ERROR_INVALID_POINTER(f == 0);
        return FALSE;
    }
    void* data = CALL(system_memory)->alloc(FILE_HANDLER_TYPE_SIZE);
    u64 address = CALL(pointer)->alloc(data, FILE_HANDLER_TYPE_SIZE, type_id);
    file_handler_ptr handler = data;
    handler->file = f;
#ifdef USE_MEMORY_DEBUG_INFO
    handler->path = file_path_data;
#endif
    return address;
}

static u64 file_free(const_vm_ptr vm, u64 address) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    file_type_destructor(address);
    return TRUE;
}

static u64 file_data(const_vm_ptr vm, u64 address) {
    if (vm == 0 || *vm == 0) {
        ERROR_VM_NOT_INITIALIZED(vm == 0 || *vm == 0);
        return FALSE;
    }
    const_void_ptr const_data_ptr = CALL(pointer)->read(address, type_id);
    safe_void_ptr void_ptr;
    void_ptr.const_ptr = const_data_ptr;
    void* data_ptr = void_ptr.ptr;
    if (data_ptr == 0) {
        ERROR_INVALID_POINTER(data_ptr == 0);
        return FALSE;
    }
    file_handler_ptr handler = data_ptr;
    FILE* f = handler->file;
    api->fseek(f, 0, SEEK_END); /* NOLINT */
    u64 size = (u64)api->ftell(f);
    api->fseek(f, 0, SEEK_SET);
    u64 data_size = size + 1;
    u64 data_handle = CALL(data)->alloc(vm, data_size);
    void* file_data = CALL(data)->unsafe(vm, data_handle);
    u64 read = api->fread(file_data, 1, size, handler->file);
    return read ? data_handle : 0;
}

CVM_EXPORT void file_init(void) {
    init();
}

/* public */
/*! definition (initialization) of file_methods structure */
const virtual_file_methods PRIVATE_API(virtual_file_methods_definitions) = {
    .alloc = file_alloc,
    .data = file_data,
    .free = file_free
};

const virtual_file_methods* file = &PRIVATE_API(virtual_file_methods_definitions);

const virtual_file_methods* CALL(file) {
    return file;
}
