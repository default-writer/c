/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 10, 2025 at 5:22:14 PM GMT+3
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

#include "vm/pointer/pointer_v1.h"
#include "vm/types/data/data_v1.h"
#include "vm/virtual/virtual_v1.h"

#include <stdio.h>

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_FILE;

#ifndef ATTRIBUTE
void file_init(void);
#endif

/* internal */
static u64 file_alloc(u64 file_path_ptr, u64 mode_ptr);
static void file_free(u64 ptr);
static u64 file_data(u64 ptr);

/* destructor */
static void virtual_free(pointer_ptr ptr);

/* implementation */
struct file_handler {
    FILE* file;
#ifdef USE_MEMORY_DEBUG_INFO
    char* path;
#endif
};

static u64 file_alloc(u64 file_path, u64 mode) {
    if (file_path == 0) {
        return 0;
    }
    if (mode == 0) {
        return 0;
    }
    const_pointer_ptr file_path_ptr = virtual->read_type(file_path, TYPE_STRING);
    if (file_path_ptr == 0) {
        return 0;
    }
    const_pointer_ptr mode_ptr = virtual->read_type(mode, TYPE_STRING);
    if (mode_ptr == 0) {
        return 0;
    }
    const char* file_path_data = pointer->read(file_path_ptr);
    const char* mode_data = pointer->read(mode_ptr);
    FILE* f = fopen(file_path_data, mode_data); /* NOLINT */
    if (f == 0) {
        return 0;
    }
    pointer_ptr f_ptr = pointer->alloc(sizeof(struct file_handler), id);
    struct file_handler* handler = pointer->read(f_ptr);
    handler->file = f;
#ifdef USE_MEMORY_DEBUG_INFO
    handler->path = pointer->read(file_path_ptr);
#endif
    u64 data_ptr = virtual->alloc(f_ptr);
    return data_ptr;
}

static void file_free(u64 ptr) {
    pointer_ptr data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    virtual_free(data_ptr);
}

static void virtual_free(pointer_ptr ptr) {
    struct file_handler* handler = pointer->read(ptr);
    if (handler->file != 0) {
        fclose(handler->file);
        handler->file = 0;
    }
    pointer->release(ptr);
}

static u64 file_data(u64 ptr) {
    const_pointer_ptr data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct file_handler* handler = pointer->read(data_ptr);
    FILE* f = handler->file;
    fseek(f, 0, SEEK_END); /* NOLINT */
    u64 size = (u64)ftell(f);
    fseek(f, 0, SEEK_SET);
    u64 data_size = size + 1;
    u64 data_handle = data->alloc(data_size);
    void* file_data = data->unsafe(data_handle);
    fread(file_data, 1, size, handler->file);
    return data_handle;
}

static const struct type_methods_definitions _type = {
    .free = virtual_free
};

static void INIT init(void) {
    pointer->register_type(id, &_type);
}

/* public */
/*! definition (initialization) of file_methods structure */
const file_methods PRIVATE_API(file_methods_definitions) = {
    .alloc = file_alloc,
    .data = file_data,
    .free = file_free
};

#ifndef ATTRIBUTE
void file_init(void) {
    init();
}
#endif
