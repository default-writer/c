/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include "common/memory.h"
#include "list-micro/data.h"

#include "../../pointer/pointer_v1.h"
#include "../../virtual/virtual_v1.h"
#include "../types_v1.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_FILE;

/* api */
const struct file_methods file_methods_definition;

#ifndef ATTRIBUTE
void file_init(void);
#endif

/* definition */
static const struct vm_type type_definition;
static const struct vm_type* type = &type_definition;

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
    const struct pointer* file_path_ptr = virtual->read_type(file_path, TYPE_STRING);
    if (file_path_ptr == 0) {
        return 0;
    }
    const struct pointer* mode_ptr = virtual->read_type(mode, TYPE_STRING);
    if (mode_ptr == 0) {
        return 0;
    }
    const char* file_path_data = pointer->read(file_path_ptr);
    const char* mode_data = pointer->read(mode_ptr);
    FILE* f = fopen(file_path_data, mode_data); /* NOLINT */
    if (f == 0) {
        return 0;
    }
    struct pointer* f_ptr = pointer->alloc(sizeof(struct file_handler), id);
    struct file_handler* handler = pointer->read(f_ptr);
    handler->file = f;
#ifdef USE_MEMORY_DEBUG_INFO
    handler->path = pointer->read(file_path_ptr);
#endif
    u64 data_ptr = virtual->alloc(f_ptr);
    return data_ptr;
}

static void file_free(u64 ptr) {
    struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    file_vm_free(data_ptr);
}

static void file_vm_free(struct pointer* ptr) {
    struct file_handler* handler = pointer->read(ptr);
    if (handler->file != 0) {
        fclose(handler->file);
        handler->file = 0;
    }
    pointer->release(ptr);
}

static u64 file_data(u64 ptr) {
    const struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct file_handler* handler = pointer->read(data_ptr);
    FILE* f = handler->file;
    fseek(f, 0, SEEK_END); /* NOLINT */
    u64 size = (u64)ftell(f);
    fseek(f, 0, SEEK_SET);
    u64 data_size = size + 1;
    u64 virtual_data = data->alloc(data_size);
    void* file_data = data->unsafe(virtual_data);
    fread(file_data, 1, size, handler->file);
    return virtual_data;
}

static const struct vm_type type_definition = {
    .free = file_vm_free
};

static void INIT init(void) {
    pointer->register_type(id, type);
}

/* public */
/*! definition (initialization) of file_methods structure */
const struct file_methods file_methods_definition = {
    .alloc = file_alloc,
    .data = file_data,
    .free = file_free
};

#ifndef ATTRIBUTE
void file_init(void) {
    init();
}
#endif
