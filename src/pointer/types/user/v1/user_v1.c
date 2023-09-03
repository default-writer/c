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
#include "pointer/types/user/v1/user_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "pointer/v1/pointer_v1.h"
#include "vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

static u64 id = TYPE_NULL;

/* api */
const struct user_methods user_methods_definition;
void user_type_init();

extern u64 pointer_vm_register_type(u64 id, const struct vm_type* type);

/* definition */
extern const struct vm_methods vm_methods_definition;
extern const struct list list_micro_definition;
extern const struct pointer_vm_methods pointer_vm_methods_definition;

/* definition */
static const struct vm_methods* vm = &vm_methods_definition;
static const struct list* list = &list_micro_definition;
static const struct pointer_vm_methods* virtual = &pointer_vm_methods_definition;

/* definition */
extern struct pointer_data vm_pointer;
static struct pointer_data* base = &vm_pointer;

/* definition */
static struct vm_type type_definition;
static const struct vm_type* type = &type_definition;

/* internal */
static u64 user_alloc(void);
static void user_free(u64 ptr);
static void user_vm_free(struct pointer* ptr);

/* implementation */
static u64 user_alloc(void) {
    struct pointer* ptr = virtual->alloc(0, id);
    u64 data = vm->alloc(ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void user_free(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    return user_vm_free(data_ptr);
}

static void user_vm_free(struct pointer* ptr) {
    virtual->free(ptr);
}

static struct vm_type type_definition = {
    .free = user_vm_free
};

static void INIT init() {
    id = pointer_vm_register_type(TYPE_NULL, type);
}

/* public */
const struct user_methods user_methods_definition = {
    .alloc = user_alloc,
    .free = user_free
};

#ifndef ATTRIBUTE
void user_type_init() {
    init();
}
#endif
