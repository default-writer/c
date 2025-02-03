/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 3, 2025 at 7:56:44 PM GMT+3
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

#define USING_VM_V1

#include "file_v1.h"

#include "std/api.h"

#include "vm/v1/pointer/pointer_v1.h"
#include "vm/v1/types/data/data_v1.h"
#include "vm/v1/virtual/virtual_v1.h"
#include "vm/v1/vm_type.h"
#include "vm/v1/vm_v1.h"
#include "vm/vm_type.h"

#include <stdio.h>

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_FILE;

#ifndef ATTRIBUTE
void file_init(void);
#endif

/* internal */
static u64 file_alloc(u64 file_path_ptr, u64 mode_ptr);
static void file_free(u64 ptr);
static void file_vm_free(struct pointer* ptr);
static u64 file_data(u64 ptr);

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
    const struct pointer* file_path_ptr = virtual_v1->read_type(file_path, TYPE_STRING);
    if (file_path_ptr == 0) {
        return 0;
    }
    const struct pointer* mode_ptr = virtual_v1->read_type(mode, TYPE_STRING);
    if (mode_ptr == 0) {
        return 0;
    }
    const char* file_path_data = pointer_v1->read(file_path_ptr);
    const char* mode_data = pointer_v1->read(mode_ptr);
    FILE* f = fopen(file_path_data, mode_data); /* NOLINT */
    if (f == 0) {
        return 0;
    }
    struct pointer* f_ptr = pointer_v1->alloc(sizeof(struct file_handler), id);
    struct file_handler* handler = pointer_v1->read(f_ptr);
    handler->file = f;
#ifdef USE_MEMORY_DEBUG_INFO
    handler->path = pointer_v1->read(file_path_ptr);
#endif
    u64 data_ptr = virtual_v1->alloc(f_ptr);
    return data_ptr;
}

static void file_free(u64 ptr) {
    struct pointer* data_ptr = virtual_v1->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    file_vm_free(data_ptr);
}

static void file_vm_free(struct pointer* ptr) {
    struct file_handler* handler = pointer_v1->read(ptr);
    if (handler->file != 0) {
        fclose(handler->file);
        handler->file = 0;
    }
    pointer_v1->release(ptr);
}

static u64 file_data(u64 ptr) {
    const struct pointer* data_ptr = virtual_v1->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct file_handler* handler = pointer_v1->read(data_ptr);
    FILE* f = handler->file;
    fseek(f, 0, SEEK_END); /* NOLINT */
    u64 size = (u64)ftell(f);
    fseek(f, 0, SEEK_SET);
    u64 data_size = size + 1;
    u64 virtual_data = type_data_v1->alloc(data_size);
    void* file_data = type_data_v1->unsafe(virtual_data);
    fread(file_data, 1, size, handler->file);
    return virtual_data;
}

static const struct vm_type type = {
    .free = file_vm_free
};

static void INIT init(void) {
    pointer_v1->register_type(id, &type);
}

/* public */
/*! definition (initialization) of file_methods structure */
const file_methods PRIVATE_API(file_methods_definition) = {
    .alloc = file_alloc,
    .data = file_data,
    .free = file_free
};

#ifndef ATTRIBUTE
void file_init(void) {
    init();
}
#endif
