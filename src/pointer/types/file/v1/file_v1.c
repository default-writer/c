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
#include "pointer/types/file/v1/file_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "pointer/v1/pointer_v1.h"
#include "vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

/* api */
const struct file_methods file_methods_definition;
void file_init();

/* definition */
extern void pointer_vm_register_type(const struct vm_type* type);
extern struct pointer_data vm_pointer;
extern const struct vm_methods vm_methods_definition;
extern const struct list list_micro_definition;
extern const struct pointer_vm_methods pointer_vm_methods_definition;
extern const struct data_methods data_methods_definition;

/* definition */
static const struct vm_methods* vm = &vm_methods_definition;
static const struct list* list = &list_micro_definition;
static const struct pointer_vm_methods* virtual = &pointer_vm_methods_definition;
static const struct data_methods* data = &data_methods_definition;
static struct pointer_data* base = &vm_pointer;
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
    struct pointer* file_path_ptr = vm->read_type(file_path, TYPE_STRING);
    if (file_path_ptr == 0) {
        return 0;
    }
    const struct pointer* mode_ptr = vm->read_type(mode, TYPE_STRING);
    if (mode_ptr == 0) {
        return 0;
    }
    const char* file_path_data = file_path_ptr->data;
    const char* mode_data = mode_ptr->data;
    FILE* file = fopen(file_path_data, mode_data); /* NOLINT */
    if (file == 0) {
        return 0;
    }
    struct pointer* f_ptr = virtual->alloc(sizeof(struct file_handler), type->id);
    struct file_handler* handler = f_ptr->data;
    handler->file = file;
#ifdef USE_MEMORY_DEBUG_INFO
    handler->path = file_path_ptr->data;
#endif
    u64 data_ptr = vm->alloc(f_ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data_ptr);
#endif
    return data_ptr;
}

static void file_free(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, type->id);
    if (data_ptr == 0) {
        return;
    }
    file_vm_free(data_ptr);
}

static void file_vm_free(struct pointer* ptr) {
    struct file_handler* handler = ptr->data;
    if (handler->file != 0) {
        fclose(handler->file);
        handler->file = 0;
    }
    virtual->free(ptr);
}

static u64 file_data(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, type->id);
    if (data_ptr == 0) {
        return 0;
    }
    struct file_handler* handler = data_ptr->data;
    FILE* file = handler->file;
    fseek(file, 0, SEEK_END); /* NOLINT */
    u64 size = (u64)ftell(file);
    fseek(file, 0, SEEK_SET);
    u64 data_size = size + 1;
    u64 virtual_data = data->alloc(data_size);
    void* file_data = data->unsafe(virtual_data);
    fread(file_data, 1, size, handler->file);
#ifdef USE_GC
    list->push(&base->gc, (void*)virtual_data);
#endif
    return virtual_data;
}

static const struct vm_type type_definition = {
    .free = file_vm_free,
    .id = TYPE_FILE
};

static void INIT init() {
    pointer_vm_register_type(type);
}

/* public */
const struct file_methods file_methods_definition = {
    .alloc = file_alloc,
    .data = file_data,
    .free = file_free
};

#ifndef ATTRIBUTE
void file_init() {
    init();
}
#endif
