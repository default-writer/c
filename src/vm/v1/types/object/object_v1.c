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

static const enum type id = TYPE_OBJECT;

/* api */
const struct object_methods object_methods_definition;

#ifndef ATTRIBUTE
void object_init(void);
#endif

/* definition */
static const struct vm_type type_definition;
static const struct vm_type* type = &type_definition;

/* internal */
static u64 object_alloc(u64 size);
static void object_free(u64 ptr);
static void object_vm_free(struct pointer* ptr);
static void* object_unsafe(u64 ptr);
static u64 object_load(const void* data, u64 size);
static u64 object_size(u64 ptr);

/* implementation */
static u64 object_alloc(u64 size) {
    if (size == 0) {
        return 0;
    }
    struct pointer* ptr = pointer->alloc(size, id);
    u64 virtual_ptr = virtual->alloc(ptr);
    return virtual_ptr;
}

static void object_free(u64 ptr) {
    struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    object_vm_free(data_ptr);
}

static void object_vm_free(struct pointer* ptr) {
    pointer->release(ptr);
}

static void* object_unsafe(u64 ptr) {
    const struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    void* object_data = pointer->read(data_ptr);
    return object_data;
}

static u64 object_load(const void* src_data, u64 size) {
    if (src_data == 0) {
        return 0;
    }
    if (size == 0) {
        return 0;
    }
    struct pointer* data_ptr = pointer->alloc(size, id);
    memcpy(pointer->read(data_ptr), src_data, size); /* NOLINT */
    u64 virtual_ptr = virtual->alloc(data_ptr);
    return virtual_ptr;
}

static u64 object_size(u64 ptr) {
    const struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    u64 size = pointer->size(data_ptr);
    return size;
}

static const struct vm_type type_definition = {
    .free = object_vm_free
};

static void INIT init(void) {
    pointer->register_type(id, type);
}

/* public */
const struct object_methods object_methods_definition = {
    .alloc = object_alloc,
    .free = object_free,
    .load = object_load,
    .unsafe = object_unsafe,
    .size = object_size
};

#ifndef ATTRIBUTE
void object_init(void) {
    init();
}
#endif
