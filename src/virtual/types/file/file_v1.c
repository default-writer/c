/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 5, 2025 at 11:57:11 PM GMT+3
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

static const enum type id = TYPE_FILE;

/* definition */
struct file_handler {
    FILE* file;
#ifdef USE_MEMORY_DEBUG_INFO
    char* path;
#endif
};

/* internal */
static u64 file_alloc(u64 file_path_ptr, u64 mode_ptr);
static void file_free(u64 ptr);
static u64 file_data(u64 ptr);

/* destructor */
static void type_desctructor(pointer_ptr ptr);

/* implementation */
static const struct type_methods_definitions file_type = {
    .desctructor = type_desctructor
};

static void INIT init(void) {
    CALL(pointer)->register_known_type(id, &file_type);
}

static void type_desctructor(pointer_ptr ptr) {
    struct file_handler* handler = CALL(pointer)->read(ptr);
    if (handler->file != 0) {
        virtual_api->fclose(handler->file);
        handler->file = 0;
    }
    CALL(pointer)->release(ptr);
}

static int is_valid_fopen_mode(const char* mode) {
    const char* valid_modes[] = { "r", "w", "a", "r+", "w+", "a+", "rb", "wb", "ab", "r+b", "w+b", "a+b" };
    size_t num_modes = sizeof(valid_modes) / sizeof(valid_modes[0]);
    for (size_t i = 0; i < num_modes; ++i) {
        if (virtual_api->strcmp(mode, valid_modes[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

static u64 file_alloc(u64 file_path, u64 mode) {
    if (file_path == 0) {
        return 0;
    }
    if (mode == 0) {
        return 0;
    }
    pointer_ptr ptr = CALL(virtual)->read_type(file_path, TYPE_STRING);
    if (ptr == 0) {
        return 0;
    }
    const_pointer_ptr file_path_ptr = ptr;
    pointer_ptr mode_ptr = CALL(virtual)->read_type(mode, TYPE_STRING);
    if (mode_ptr == 0) {
        return 0;
    }
    const char* file_path_data = CALL(pointer)->read(file_path_ptr);
    const char* mode_data = CALL(pointer)->read(mode_ptr);
    if (!is_valid_fopen_mode(mode_data)) {
        return 0;
    }
    FILE* f = virtual_api->fopen(file_path_data, mode_data); /* NOLINT */
    if (f == 0) {
        return 0;
    }
    pointer_ptr f_ptr = CALL(pointer)->alloc(sizeof(struct file_handler), id);
    struct file_handler* handler = CALL(pointer)->read(f_ptr);
    handler->file = f;
#ifdef USE_MEMORY_DEBUG_INFO
    handler->path = CALL(pointer)->read(file_path_ptr);
#endif
    u64 data_ptr = CALL(virtual)->alloc(f_ptr);
    return data_ptr;
}

static void file_free(u64 ptr) {
    pointer_ptr data_ptr = CALL(virtual)->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    type_desctructor(data_ptr);
}

static u64 file_data(u64 ptr) {
    pointer_ptr data_ptr = CALL(virtual)->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct file_handler* handler = CALL(pointer)->read(data_ptr);
    FILE* f = handler->file;
    virtual_api->fseek(f, 0, SEEK_END); /* NOLINT */
    u64 size = (u64)virtual_api->ftell(f);
    virtual_api->fseek(f, 0, SEEK_SET);
    u64 data_size = size + 1;
    u64 data_handle = CALL(data)->alloc(data_size);
    void* file_data = CALL(data)->unsafe(data_handle);
    u64 read = virtual_api->fread(file_data, 1, size, handler->file);
    return read ? data_handle : 0;
}

#ifndef ATTRIBUTE
void file_init(void) {
    init();
}
#endif

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
