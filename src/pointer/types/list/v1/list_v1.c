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
#include "pointer/types/list/v1/list_v1.h"
#include "common/alloc.h"
#include "list-micro/data.h"
#include "pointer/v1/pointer_v1.h"
#include "vm/v1/vm_v1.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_LIST;

/* api */
const struct list_methods list_methods_definition;
void list_init();

/* definition */
extern u64 pointer_vm_register_type(u64 id, const struct vm_type* type);
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
static u64 list_alloc(void);
static void list_free(u64 ptr);
static void list_vm_free(struct pointer* ptr);
static void list_push(u64 ptr, u64 data_ptr);
static u64 list_peek(u64 ptr);
static u64 list_pop(u64 ptr);
static u64 list_size(u64 ptr);

/* definition */
struct list_handler {
    u64 size;
    struct list_data* list;
};

/* implementation */
static u64 list_alloc(void) {
    struct pointer* ptr = virtual->alloc(sizeof(struct list_handler), id);
    struct list_handler* handler = ptr->data;
    handler->size = 0;
    list->init(&handler->list);
    u64 data = vm->alloc(ptr);
#ifdef USE_GC
    list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void list_release(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    struct list_handler* handler = data_ptr->data;
    handler->size = 0;
    virtual->release(&handler->list);
}

static void list_free(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    list_vm_free(data_ptr);
}

static void list_vm_free(struct pointer* ptr) {
    struct list_handler* handler = ptr->data;
    handler->size = 0;
    virtual->release(&handler->list);
    list->destroy(&handler->list);
    virtual->free(ptr);
}

static void list_push(u64 ptr_list, u64 ptr) {
    if (ptr_list == ptr) {
        return;
    }
    if (ptr_list == 0) {
        return;
    }
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = vm->read_type(ptr_list, id);
    if (data_ptr == 0) {
        return;
    }
    struct list_handler* handler = data_ptr->data;
    list->push(&handler->list, (void*)ptr);
    handler->size++;
#ifdef USE_GC
    list->push(&base->gc, (void*)ptr);
#endif
}

static u64 list_peek(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct list_handler* handler = data_ptr->data;
    u64 data = (u64)list->peek(&handler->list);
    return data;
}

static u64 list_pop(u64 ptr) {
    struct pointer* data_ptr = vm->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct list_handler* handler = data_ptr->data;
    if (handler->size == 0) {
        return 0;
    }
    u64 data = (u64)list->pop(&handler->list);
    handler->size--;
    return data;
}

static u64 list_size(u64 ptr) {
    const struct pointer* data_ptr = vm->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct list_handler* handler = data_ptr->data;
    u64 size = handler->size;
    return size;
}

static const struct vm_type type_definition = {
    .free = list_vm_free
};

static void INIT init() {
    pointer_vm_register_type(id, type);
}

/* public */
const struct list_methods list_methods_definition = {
    .alloc = list_alloc,
    .free = list_free,
    .push = list_push,
    .peek = list_peek,
    .pop = list_pop,
    .size = list_size,
    .release = list_release
};

#ifndef ATTRIBUTE
void list_init() {
    init();
}
#endif
