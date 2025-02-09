/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 9, 2025 at 11:07:50 AM GMT+3
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

#include "sys/list/list_v1.h"

#include "vm/v1/pointer/pointer_v1.h"
#include "vm/v1/virtual/virtual_v1.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_STACK;

#ifndef ATTRIBUTE
void stack_init(void);
#endif

/* internal */
static pointer_ptr stack_alloc_internal(void);
static void stack_release_internal(stack_ptr* curent);

/* api */
static u64 stack_alloc(void);
static void stack_free(u64 ptr);
static void stack_push(u64 ptr, u64 data_ptr);
static u64 stack_peek(u64 ptr);
static u64 stack_peekn(u64 ptr, u64 nelements);
static u64 stack_pop(u64 ptr);
static u64 stack_popn(u64 ptr, u64 nelements);
static u64 stack_size(u64 ptr);
static void stack_release(u64 ptr);

/* destructor */
static void virtual_free(pointer_ptr ptr);

/* definition */
struct stack_handler {
    u64 size;
    stack_ptr list;
};

/* implementation */
static pointer_ptr stack_alloc_internal(void) {
    pointer_ptr ptr = pointer->alloc(sizeof(struct stack_handler), id);
    struct stack_handler* handler = pointer->read(ptr);
    handler->size = 0;
    sys_list->init(&handler->list);
    return ptr;
}

static void stack_release_internal(stack_ptr* current) {
    u64 ptr = 0;
    while ((ptr = (u64)sys_list->pop(current)) != 0) {
        pointer->free(ptr);
    }
}

static u64 stack_alloc(void) {
    pointer_ptr ptr = stack_alloc_internal();
    u64 virtual_ptr = virtual->alloc(ptr);
    return virtual_ptr;
}

static void stack_release(u64 ptr) {
    const_pointer_ptr data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    struct stack_handler* handler = pointer->read(data_ptr);
    handler->size = 0;
    stack_release_internal(&handler->list);
}

static void stack_free(u64 ptr) {
    pointer_ptr data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    virtual_free(data_ptr);
}

static void virtual_free(pointer_ptr ptr) {
    struct stack_handler* handler = pointer->read(ptr);
    handler->size = 0;
    stack_release_internal(&handler->list);
    sys_list->destroy(&handler->list);
    pointer->release(ptr);
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
    const_pointer_ptr data_ptr = virtual->read_type(ptr_list, id);
    if (data_ptr == 0) {
        return;
    }
    struct stack_handler* handler = pointer->read(data_ptr);
    sys_list->push(&handler->list, (void*)ptr);
    handler->size++;
}

static u64 stack_peek(u64 ptr) {
    const_pointer_ptr data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct stack_handler* handler = pointer->read(data_ptr);
    u64 stack_peek_ptr = (u64)sys_list->peek(&handler->list);
    return stack_peek_ptr;
}

static u64 stack_peekn(u64 ptr, u64 nelements) {
    const_pointer_ptr data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct stack_handler* src_handler = pointer->read(data_ptr);
    u64 size = src_handler->size;
    if (size == 0) {
        return 0;
    }
    if (size < nelements) {
        return 0;
    }
    pointer_ptr dst_ptr = stack_alloc_internal();
    struct stack_handler* dst_handler = pointer->read(dst_ptr);
    u64 i = nelements;
    while (i-- > 0) {
        stack_ptr current = src_handler->list;
        u64 stack_peek_ptr = (u64)sys_list->peek(&current);
        sys_list->push(&dst_handler->list, (void*)stack_peek_ptr);
        dst_handler->size++;
        current = current->next;
    }
    u64 dst_data = virtual->alloc(dst_ptr);
    return dst_data;
}

static u64 stack_pop(u64 ptr) {
    const_pointer_ptr data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct stack_handler* handler = pointer->read(data_ptr);
    if (handler->size == 0) {
        return 0;
    }
    u64 stack_pop_ptr = (u64)sys_list->pop(&handler->list);
    handler->size--;
    return stack_pop_ptr;
}

static u64 stack_popn(u64 ptr, u64 nelements) {
    const_pointer_ptr data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct stack_handler* src_handler = pointer->read(data_ptr);
    u64 size = src_handler->size;
    if (size == 0) {
        return 0;
    }
    if (size < nelements) {
        return 0;
    }
    pointer_ptr dst_ptr = stack_alloc_internal();
    struct stack_handler* dst_handler = pointer->read(dst_ptr);
    u64 i = nelements;
    while (i-- > 0) {
        u64 stack_pop_ptr = (u64)sys_list->pop(&src_handler->list);
        sys_list->push(&dst_handler->list, (void*)stack_pop_ptr);
        dst_handler->size++;
    }
    u64 dst_data = virtual->alloc(dst_ptr);
    return dst_data;
}

static u64 stack_size(u64 ptr) {
    const_pointer_ptr data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    const struct stack_handler* handler = pointer->read(data_ptr);
    u64 size = handler->size;
    return size;
}

static const struct type_methods_definitions _type = {
    .free = virtual_free
};

static void INIT init(void) {
    pointer->register_type(id, &_type);
}

/* public */
const stack_methods PRIVATE_API(stack_methods_definitions) = {
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
