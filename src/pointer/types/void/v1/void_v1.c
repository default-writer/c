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
#include "pointer/types/void/v1/void_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "pointer/v1/pointer_v1.h"
#include "vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

/* api */
const struct void_methods void_methods_definition;
void void_init();

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
static u64 void_alloc(u64 size);
static void void_free(u64 ptr);
static void void_vm_free(struct pointer* ptr);

/* implementation*/
static u64 void_alloc(u64 size) {
    if (size == 0) {
        return 0;
    }
    struct pointer* ptr = virtual->alloc(size, type->id);
    u64 data = vm->alloc(ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void void_free(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, type->id);
    if (data_ptr == 0) {
        return;
    }
    return void_vm_free(data_ptr);
}

static void* void_unsafe(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, type->id);
    if (data_ptr == 0) {
        return 0;
    }
    void* data = data_ptr->data;
    return data;
}

static void void_vm_free(struct pointer* ptr) {
    virtual->free(ptr);
}

static const struct vm_type type_definition = {
    .free = void_vm_free,
    .id = TYPE_VOID
};

static void INIT init() {
    pointer_vm_register_type(type);
}

/* public */
const struct void_methods void_methods_definition = {
    .alloc = void_alloc,
    .free = void_free,
    .unsafe = void_unsafe
};

#ifndef ATTRIBUTE
void void_init() {
    init();
}
#endif