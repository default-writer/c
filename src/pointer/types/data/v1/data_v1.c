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
#include "pointer/types/data/v1/data_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "pointer/v1/pointer_v1.h"
#include "vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

/* api */
const struct data_methods data_methods_definition;
void data_init();

/* definition */
extern void pointer_vm_register_type(const struct vm_type* type);
extern struct pointer_data vm_pointer;
extern const struct vm_methods vm_methods_definition;
extern const struct list list_micro_definition;
extern const struct pointer_vm_methods pointer_vm_methods_definition;

/* definition */
static const struct vm_methods* vm = &vm_methods_definition;
static const struct list* list = &list_micro_definition;
static const struct pointer_vm_methods* virtual = &pointer_vm_methods_definition;
static struct pointer_data* base = &vm_pointer;
static const struct vm_type type_definition;
static const struct vm_type* type = &type_definition;

/* internal */
static u64 data_alloc(u64 size);
static void data_free(u64 ptr);
static void data_vm_free(struct pointer* data_ptr);
static void* data_unsafe(u64 ptr);
static u64 data_size(u64 ptr);

/* implementation */
static u64 data_alloc(u64 size) {
    struct pointer* f_ptr = virtual->alloc(size, type->id);
    u64 data = vm->alloc(f_ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void data_free(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, type->id);
    if (data_ptr == 0) {
        return;
    }
    data_vm_free(data_ptr);
}

static void data_vm_free(struct pointer* ptr) {
    virtual->free(ptr);
}

static void* data_unsafe(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, type->id);
    if (data_ptr == 0) {
        return 0;
    }
    void* data = data_ptr->data;
    return data;
}

static u64 data_size(u64 ptr) {
    const struct pointer* data_ptr = vm->read_type(ptr, type->id);
    if (data_ptr == 0) {
        return 0;
    }
    u64 size = data_ptr->size;
    return size;
}

static const struct vm_type type_definition = {
    .free = data_vm_free,
    .id = TYPE_DATA
};

static void INIT init() {
    pointer_vm_register_type(type);
}

/* public */
const struct data_methods data_methods_definition = {
    .alloc = data_alloc,
    .free = data_free,
    .unsafe = data_unsafe,
    .size = data_size
};

#ifndef ATTRIBUTE
void data_init() {
    init();
}
#endif