/*
 *
 *  MIT License
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
#include "pointer/types/list/v1/list_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "pointer/v1/pointer_v1.h"
#include "vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

/* api */
const struct list_methods list_methods_definition;
void list_init();

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

/* internal */
static u64 list_alloc(void);
static u64 list_free(u64 ptr);
static void list_push(u64 ptr, u64 data_ptr);
static u64 list_peek(u64 ptr);
static u64 list_pop(u64 ptr);

/* implementation*/
struct list_handler {
    struct list_data* list;
};

static u64 list_alloc(void) {
    struct pointer* ptr = virtual->alloc(sizeof(struct list_handler), TYPE_LIST);
    struct list_handler* handler = ptr->data;
    list->init(&handler->list);
    u64 data = vm->alloc(ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static u64 list_free(u64 ptr) {
    struct pointer* data_ptr = vm->read(ptr, TYPE_LIST);
    if (data_ptr == 0) {
        return 0;
    }
    struct list_handler* handler = data_ptr->data;
    virtual->cleanup(&handler->list);
    list->destroy(&handler->list);
    virtual->free(data_ptr);
    return TYPE_LIST;
}

static void list_push(u64 ptr_list, u64 ptr) {
    if (ptr_list == 0) {
        return;
    }
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = vm->read(ptr_list, TYPE_LIST);
    if (data_ptr == 0) {
        return;
    }
    struct list_handler* handler = data_ptr->data;
    list->push(&handler->list, (void*)ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)ptr);
#endif
}

static u64 list_peek(u64 ptr) {
    struct pointer* data_ptr = vm->read(ptr, TYPE_LIST);
    if (data_ptr == 0) {
        return 0;
    }
    struct list_handler* handler = data_ptr->data;
    u64 data = (u64)list->peek(&handler->list);
    return data;
}

static u64 list_pop(u64 ptr) {
    struct pointer* data_ptr = vm->read(ptr, TYPE_LIST);
    if (data_ptr == 0) {
        return 0;
    }
    struct list_handler* handler = data_ptr->data;
    u64 data = (u64)list->pop(&handler->list);
    return data;
}

static u64 list_size(u64 ptr) {
    struct pointer* data_ptr = vm->read(ptr, TYPE_LIST);
    if (data_ptr == 0) {
        return 0;
    }
    u64 size = data_ptr->size;
    return size;
}

static const struct vm_type type_definition = {
    .free = list_free,
    .typeid = TYPE_LIST
};

static void INIT init() {
    const struct vm_type* type = &type_definition;
    pointer_vm_register_type(type);
}

/* public */
const struct list_methods list_methods_definition = {
    .alloc = list_alloc,
    .free = list_free,
    .push = list_push,
    .peek = list_peek,
    .pop = list_pop,
    .size = list_size
};

#ifndef ATTRIBUTE
void list_init() {
    init();
}
#endif
