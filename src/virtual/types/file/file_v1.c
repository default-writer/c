/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 10, 2025 at 1:48:58 AM GMT+3
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

#include "std/api.h"

#include "virtual/api/api_v1.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/data/data_v1.h"
#include "virtual/virtual/virtual_v1.h"

#include <stdio.h>
#include <string.h>

#define DEFAULT_SIZE 0x100
#define FILE_HANDLER_SIZE sizeof(file_handler_type)

static const enum type id = TYPE_FILE;

/* definition */
typedef struct file_handler {
    FILE* file;
#ifdef USE_MEMORY_DEBUG_INFO
    char* path;
#endif
} file_handler_type;

/* internal */
static u64 file_alloc(const_vm_ptr vm, u64 file_path_ptr, u64 mode_ptr);
static void file_free(const_vm_ptr vm, u64 ptr);
static u64 file_data(const_vm_ptr vm, u64 ptr);

/* destructor */
static void type_desctructor(const_pointer_ptr const_ptr);

/* implementation */
static struct type_methods_definitions file_type = {
    .desctructor = type_desctructor
};

static void INIT init(void) {
    safe_type_methods_definitions safe_ptr;
    safe_ptr.const_ptr = &file_type;
    CALL(pointer)->register_known_type(id, safe_ptr.ptr);
}

static void type_desctructor(const_pointer_ptr const_ptr) {
    struct file_handler* handler = CALL(pointer)->read(const_ptr);
    if (handler->file != 0) {
        virtual_api->fclose(handler->file);
        handler->file = 0;
    }
    CALL(pointer)->release(const_ptr);
}

static int is_valid_file_path(const char* file_path) {
    size_t size = virtual_api->strlen(file_path);
    if (size > 0 && size <= PATH_MAX - 1) {
        return 1;
    }
    return 0;
}

static int is_valid_mode(const char* mode) {
    const char* valid_modes[] = { "r", "w", "a", "r+", "w+", "a+", "rb", "wb", "ab", "r+b", "w+b", "a+b" };
    size_t num_modes = sizeof(valid_modes) / sizeof(valid_modes[0]);
    for (size_t i = 0; i < num_modes; ++i) {
        if (virtual_api->strcmp(mode, valid_modes[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

static u64 file_alloc(const_vm_ptr vm, u64 file_path, u64 mode) {
    if (vm == 0 || *vm == 0) {
        return 0;
    }
    if (file_path == 0) {
        return 0;
    }
    if (mode == 0) {
        return 0;
    }
    const_pointer_ptr file_path_ptr = CALL(virtual)->read_type(vm, file_path, TYPE_STRING);
    if (file_path_ptr == 0) {
        return 0;
    }
    const_pointer_ptr mode_ptr = CALL(virtual)->read_type(vm, mode, TYPE_STRING);
    if (mode_ptr == 0) {
        return 0;
    }
    const char* file_path_data = CALL(pointer)->read(file_path_ptr);
    if (!is_valid_file_path(file_path_data)) {
        return 0;
    }
    const char* mode_data = CALL(pointer)->read(mode_ptr);
    if (!is_valid_mode(mode_data)) {
        return 0;
    }
    FILE* f = virtual_api->fopen(file_path_data, mode_data); /* NOLINT */
    if (f == 0) {
        return 0;
    }
    const_pointer_ptr f_ptr = CALL(pointer)->alloc(FILE_HANDLER_SIZE, id);
    struct file_handler* handler = CALL(pointer)->read(f_ptr);
    handler->file = f;
#ifdef USE_MEMORY_DEBUG_INFO
    handler->path = CALL(pointer)->read(file_path_ptr);
#endif
    return CALL(virtual)->alloc(vm, f_ptr);
}

static void file_free(const_vm_ptr vm, u64 ptr) {
    if (vm == 0 || *vm == 0) {
        return;
    }
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(vm, ptr, id);
    if (data_ptr == 0) {
        return;
    }
    type_desctructor(data_ptr);
}

static u64 file_data(const_vm_ptr vm, u64 ptr) {
    if (vm == 0 || *vm == 0) {
        return 0;
    }
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(vm, ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct file_handler* handler = CALL(pointer)->read(data_ptr);
    FILE* f = handler->file;
    virtual_api->fseek(f, 0, SEEK_END); /* NOLINT */
    u64 size = (u64)virtual_api->ftell(f);
    virtual_api->fseek(f, 0, SEEK_SET);
    u64 data_size = size + 1;
    u64 data_handle = CALL(data)->alloc(vm, data_size);
    void* file_data = CALL(data)->unsafe(vm, data_handle);
    u64 read = virtual_api->fread(file_data, 1, size, handler->file);
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
