/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 7, 2025 at 2:20:24 PM GMT+3
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

#include "system/list/list_v1.h"

#include "virtual/pointer/pointer_v1.h"
#include "virtual/virtual/virtual_v1.h"

#define DEFAULT_SIZE 0x100
#define STACK_HANDLER_SIZE sizeof(stack_handler_type)

static const enum type id = TYPE_STACK;

/* internal */
static const_pointer_ptr stack_alloc_internal(void);
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

/* definition */
typedef struct stack_handler* stack_handler_ptr;
typedef const struct stack_handler* const_stack_handler_ptr;
typedef struct stack_handler {
    u64 size;
    stack_ptr list;
} stack_handler_type;

/* destructor */
static void type_desctructor(const_pointer_ptr ptr);

/* implementation */
static const struct type_methods_definitions stack_type = {
    .desctructor = type_desctructor
};

static void INIT init(void) {
    CALL(pointer)->register_known_type(id, &stack_type);
}

static void type_desctructor(const_pointer_ptr ptr) {
    stack_handler_ptr handler = CALL(pointer)->read(ptr);
    handler->size = 0;
    stack_release_internal(&(handler->list));
    CALL(system_list)->destroy(&(handler->list));
    CALL(pointer)->release(ptr);
}

static const_pointer_ptr stack_alloc_internal(void) {
    const_pointer_ptr ptr = CALL(pointer)->alloc(STACK_HANDLER_SIZE, id);
    stack_handler_ptr handler = CALL(pointer)->read(ptr);
    handler->size = 0;
    CALL(system_list)->init(&handler->list);
    return ptr;
}

static void stack_release_internal(stack_ptr* current) {
    u64 ptr = 0;
    while ((ptr = (u64)CALL(system_list)->pop(current)) != 0) {
        CALL(pointer)->free(ptr);
    }
}

static u64 stack_alloc(void) {
    const_pointer_ptr ptr = stack_alloc_internal();
    u64 virtual_ptr = CALL(virtual)->alloc(ptr);
    return virtual_ptr;
}

static void stack_release(u64 ptr) {
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    stack_handler_ptr handler = CALL(pointer)->read(data_ptr);
    handler->size = 0;
    stack_release_internal(&handler->list);
}

static void stack_free(u64 ptr) {
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    type_desctructor(data_ptr);
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
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(ptr_list, id);
    if (data_ptr == 0) {
        return;
    }
    stack_handler_ptr handler = CALL(pointer)->read(data_ptr);
    CALL(system_list)->push(&handler->list, (void*)ptr);
    handler->size++;
}

static u64 stack_peek(u64 ptr) {
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    stack_handler_ptr handler = CALL(pointer)->read(data_ptr);
    u64 stack_peek_ptr = (u64)CALL(system_list)->peek(&handler->list);
    return stack_peek_ptr;
}

static u64 stack_peekn(u64 ptr, u64 nelements) {
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    stack_handler_ptr src_handler = CALL(pointer)->read(data_ptr);
    u64 size = src_handler->size;
    if (size == 0) {
        return 0;
    }
    if (size < nelements) {
        return 0;
    }
    const_pointer_ptr dst_ptr = stack_alloc_internal();
    stack_handler_ptr dst_handler = CALL(pointer)->read(dst_ptr);
    u64 i = nelements;
    while (i-- > 0) {
        stack_ptr current = src_handler->list;
        u64 stack_peek_ptr = (u64)CALL(system_list)->peek(&current);
        CALL(system_list)->push(&dst_handler->list, (void*)stack_peek_ptr);
        dst_handler->size++;
        current = current->next;
    }
    u64 dst_data = CALL(virtual)->alloc(dst_ptr);
    return dst_data;
}

static u64 stack_pop(u64 ptr) {
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    stack_handler_ptr handler = CALL(pointer)->read(data_ptr);
    if (handler->size == 0) {
        return 0;
    }
    u64 stack_pop_ptr = (u64)CALL(system_list)->pop(&handler->list);
    handler->size--;
    return stack_pop_ptr;
}

static u64 stack_popn(u64 ptr, u64 nelements) {
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    stack_handler_ptr src_handler = CALL(pointer)->read(data_ptr);
    u64 size = src_handler->size;
    if (size == 0) {
        return 0;
    }
    if (size < nelements) {
        return 0;
    }
    const_pointer_ptr dst_ptr = stack_alloc_internal();
    stack_handler_ptr dst_handler = CALL(pointer)->read(dst_ptr);
    u64 i = nelements;
    while (i-- > 0) {
        u64 stack_pop_ptr = (u64)CALL(system_list)->pop(&src_handler->list);
        CALL(system_list)->push(&dst_handler->list, (void*)stack_pop_ptr);
        dst_handler->size++;
    }
    u64 dst_data = CALL(virtual)->alloc(dst_ptr);
    return dst_data;
}

static u64 stack_size(u64 ptr) {
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    const_stack_handler_ptr handler = CALL(pointer)->read(data_ptr);
    u64 size = handler->size;
    return size;
}

#ifndef ATTRIBUTE
void stack_init(void) {
    init();
}
#endif

/* public */
const virtual_stack_methods PRIVATE_API(virtual_stack_methods_definitions) = {
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

const virtual_stack_methods* stack = &PRIVATE_API(virtual_stack_methods_definitions);

const virtual_stack_methods* CALL(stack) {
    return stack;
}
