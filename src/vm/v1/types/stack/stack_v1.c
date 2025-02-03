/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 3, 2025 at 9:46:28 PM GMT+3
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

#include "stack_v1.h"

#include "std/api.h"

#include "list/list_v1.h"

#include "vm/v1/pointer/pointer_v1.h"
#include "vm/v1/virtual/virtual_v1.h"
#include "vm/v1/vm_type.h"
#include "vm/vm_type.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_LIST;

#ifndef ATTRIBUTE
void stack_init(void);
#endif

/* internal */
static struct pointer* stack_alloc_internal(void);
static void stack_release_internal(stack_pointer* curent);

/* api */
static u64 stack_alloc(void);
static void stack_free(u64 ptr);
static void stack_vm_free(struct pointer* ptr);
static void stack_push(u64 ptr, u64 data_ptr);
static u64 stack_peek(u64 ptr);
static u64 stack_peekn(u64 stack_ptr, u64 nelements);
static u64 stack_pop(u64 ptr);
static u64 stack_popn(u64 stack_ptr, u64 nelements);
static u64 stack_size(u64 ptr);
static void stack_release(u64 ptr);

/* definition */
struct stack_handler {
    u64 size;
    stack_pointer list;
};

/* implementation */
static struct pointer* stack_alloc_internal(void) {
    struct pointer* ptr = pointer_v1->alloc(sizeof(struct stack_handler), id);
    struct stack_handler* handler = pointer_v1->read(ptr);
    handler->size = 0;
    list_v1->init(&handler->list);
    return ptr;
}

static void stack_release_internal(stack_pointer* current) {
    u64 ptr = 0;
    while ((ptr = (u64)list_v1->pop(current)) != 0) {
        pointer_v1->free(ptr);
    }
}

static u64 stack_alloc(void) {
    struct pointer* ptr = stack_alloc_internal();
    u64 virtual_ptr = virtual_v1->alloc(ptr);
    return virtual_ptr;
}

static void stack_release(u64 ptr) {
    const struct pointer* data_ptr = virtual_v1->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    struct stack_handler* handler = pointer_v1->read(data_ptr);
    handler->size = 0;
    stack_release_internal(&handler->list);
}

static void stack_free(u64 ptr) {
    struct pointer* data_ptr = virtual_v1->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    stack_vm_free(data_ptr);
}

static void stack_vm_free(struct pointer* ptr) {
    struct stack_handler* handler = pointer_v1->read(ptr);
    handler->size = 0;
    stack_release_internal(&handler->list);
    list_v1->destroy(&handler->list);
    pointer_v1->release(ptr);
}

static void stack_push(u64 ptr_list, u64 ptr) {
    if (ptr_list == ptr) {
        return;
    }
    if (ptr_list == 0) {
        return;
    }
    if (ptr == 0) {
        return;
    }
    const struct pointer* data_ptr = virtual_v1->read_type(ptr_list, id);
    if (data_ptr == 0) {
        return;
    }
    struct stack_handler* handler = pointer_v1->read(data_ptr);
    list_v1->push(&handler->list, (void*)ptr);
    handler->size++;
}

static u64 stack_peek(u64 ptr) {
    const struct pointer* data_ptr = virtual_v1->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct stack_handler* handler = pointer_v1->read(data_ptr);
    u64 stack_peek_ptr = (u64)list_v1->peek(&handler->list);
    return stack_peek_ptr;
}

static u64 stack_peekn(u64 stack_ptr, u64 nelements) {
    const struct pointer* data_ptr = virtual_v1->read_type(stack_ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct stack_handler* src_handler = pointer_v1->read(data_ptr);
    u64 size = src_handler->size;
    if (size == 0) {
        return 0;
    }
    if (size < nelements) {
        return 0;
    }
    struct pointer* dst_ptr = stack_alloc_internal();
    struct stack_handler* dst_handler = pointer_v1->read(dst_ptr);
    u64 i = nelements;
    while (i-- > 0) {
        stack_pointer current = src_handler->list;
        u64 stack_peek_ptr = (u64)list_v1->peek(&current);
        list_v1->push(&dst_handler->list, (void*)stack_peek_ptr);
        dst_handler->size++;
        current = current->next;
    }
    u64 dst_data = virtual_v1->alloc(dst_ptr);
    return dst_data;
}

static u64 stack_pop(u64 ptr) {
    const struct pointer* data_ptr = virtual_v1->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct stack_handler* handler = pointer_v1->read(data_ptr);
    if (handler->size == 0) {
        return 0;
    }
    u64 stack_pop_ptr = (u64)list_v1->pop(&handler->list);
    handler->size--;
    return stack_pop_ptr;
}

static u64 stack_popn(u64 stack_ptr, u64 nelements) {
    const struct pointer* data_ptr = virtual_v1->read_type(stack_ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct stack_handler* src_handler = pointer_v1->read(data_ptr);
    u64 size = src_handler->size;
    if (size == 0) {
        return 0;
    }
    if (size < nelements) {
        return 0;
    }
    struct pointer* dst_ptr = stack_alloc_internal();
    struct stack_handler* dst_handler = pointer_v1->read(dst_ptr);
    u64 i = nelements;
    while (i-- > 0) {
        u64 stack_pop_ptr = (u64)list_v1->pop(&src_handler->list);
        list_v1->push(&dst_handler->list, (void*)stack_pop_ptr);
        dst_handler->size++;
    }
    u64 dst_data = virtual_v1->alloc(dst_ptr);
    return dst_data;
}

static u64 stack_size(u64 ptr) {
    const struct pointer* data_ptr = virtual_v1->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    const struct stack_handler* handler = pointer_v1->read(data_ptr);
    u64 size = handler->size;
    return size;
}

static const struct vm_type type = {
    .free = stack_vm_free
};

static void INIT init(void) {
    pointer_v1->register_type(id, &type);
}

/* public */
const stack PRIVATE_API(stack_definition) = {
    .alloc = stack_alloc,
    .free = stack_free,
    .push = stack_push,
    .peek = stack_peek,
    .peekn = stack_peekn,
    .pop = stack_pop,
    .popn = stack_popn,
    .size = stack_size,
    .release = stack_release
};

#ifndef ATTRIBUTE
void stack_init(void) {
    init();
}
#endif
