/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:15:15 GMT+3
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

#include "common/memory.h"
#include "list-micro/data.h"

#include "pointer/types/list/v1/list_v1.h"
#include "pointer/types/types.h"
#include "pointer/v1/pointer_v1.h"
#include "pointer/v1/virtual_v1.h"

#define DEFAULT_SIZE 0x100

static const enum type id = TYPE_LIST;

/* api */
const struct list_methods list_methods_definition;

#ifndef ATTRIBUTE
void list_init(void);
#endif

/* definition */
extern const struct list list_micro_definition;

/* definition */
static const struct list* _list = &list_micro_definition;

/* definition */
static const struct vm_type type_definition;
static const struct vm_type* type = &type_definition;

/* internal */
static struct pointer* list_alloc_internal(void);
static void list_release_internal(struct list_data** curent);

/* api */
static u64 list_alloc(void);
static void list_free(u64 ptr);
static void list_vm_free(struct pointer* ptr);
static void list_push(u64 ptr, u64 data_ptr);
static u64 list_peek(u64 ptr);
static u64 list_peekn(u64 list_ptr, u64 nelements);
static u64 list_pop(u64 ptr);
static u64 list_popn(u64 list_ptr, u64 nelements);
static u64 list_size(u64 ptr);
static void list_release(u64 ptr);

/* definition */
struct list_handler {
    u64 size;
    struct list_data* list;
};

/* implementation */
static struct pointer* list_alloc_internal(void) {
    struct pointer* ptr = pointer->alloc(sizeof(struct list_handler), id);
    struct list_handler* handler = pointer->read(ptr);
    handler->size = 0;
    _list->init(&handler->list);
    return ptr;
}

static void list_release_internal(struct list_data** current) {
    u64 ptr = 0;
    while ((ptr = (u64)_list->pop(current)) != 0) {
        pointer->free(ptr);
    }
}

static u64 list_alloc(void) {
    struct pointer* ptr = list_alloc_internal();
    u64 virtual_ptr = virtual->alloc(ptr);
    return virtual_ptr;
}

static void list_release(u64 ptr) {
    const struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    struct list_handler* handler = pointer->read(data_ptr);
    handler->size = 0;
    list_release_internal(&handler->list);
}

static void list_free(u64 ptr) {
    struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    list_vm_free(data_ptr);
}

static void list_vm_free(struct pointer* ptr) {
    struct list_handler* handler = pointer->read(ptr);
    handler->size = 0;
    list_release_internal(&handler->list);
    _list->destroy(&handler->list);
    pointer->release(ptr);
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
    const struct pointer* data_ptr = virtual->read_type(ptr_list, id);
    if (data_ptr == 0) {
        return;
    }
    struct list_handler* handler = pointer->read(data_ptr);
    _list->push(&handler->list, (void*)ptr);
    handler->size++;
}

static u64 list_peek(u64 ptr) {
    const struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct list_handler* handler = pointer->read(data_ptr);
    u64 list_peek_ptr = (u64)_list->peek(&handler->list);
    return list_peek_ptr;
}

static u64 list_peekn(u64 list_ptr, u64 nelements) {
    const struct pointer* data_ptr = virtual->read_type(list_ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct list_handler* src_handler = pointer->read(data_ptr);
    u64 size = src_handler->size;
    if (size == 0) {
        return 0;
    }
    if (size < nelements) {
        return 0;
    }
    struct pointer* dst_ptr = list_alloc_internal();
    struct list_handler* dst_handler = pointer->read(dst_ptr);
    u64 i = nelements;
    while (i-- > 0) {
        struct list_data* current = src_handler->list;
        u64 list_peek_ptr = (u64)_list->peek(&current);
        _list->push(&dst_handler->list, (void*)list_peek_ptr);
        dst_handler->size++;
        current = current->next;
    }
    u64 dst_data = virtual->alloc(dst_ptr);
    return dst_data;
}

static u64 list_pop(u64 ptr) {
    const struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct list_handler* handler = pointer->read(data_ptr);
    if (handler->size == 0) {
        return 0;
    }
    u64 list_pop_ptr = (u64)_list->pop(&handler->list);
    handler->size--;
    return list_pop_ptr;
}

static u64 list_popn(u64 list_ptr, u64 nelements) {
    const struct pointer* data_ptr = virtual->read_type(list_ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    struct list_handler* src_handler = pointer->read(data_ptr);
    u64 size = src_handler->size;
    if (size == 0) {
        return 0;
    }
    if (size < nelements) {
        return 0;
    }
    struct pointer* dst_ptr = list_alloc_internal();
    struct list_handler* dst_handler = pointer->read(dst_ptr);
    u64 i = nelements;
    while (i-- > 0) {
        u64 list_pop_ptr = (u64)_list->pop(&src_handler->list);
        _list->push(&dst_handler->list, (void*)list_pop_ptr);
        dst_handler->size++;
    }
    u64 dst_data = virtual->alloc(dst_ptr);
    return dst_data;
}

static u64 list_size(u64 ptr) {
    const struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return 0;
    }
    const struct list_handler* handler = pointer->read(data_ptr);
    u64 size = handler->size;
    return size;
}

static const struct vm_type type_definition = {
    .free = list_vm_free
};

static void INIT init(void) {
    pointer->register_type(id, type);
}

/* public */
const struct list_methods list_methods_definition = {
    .alloc = list_alloc,
    .free = list_free,
    .push = list_push,
    .peek = list_peek,
    .peekn = list_peekn,
    .pop = list_pop,
    .popn = list_popn,
    .size = list_size,
    .release = list_release
};

#ifndef ATTRIBUTE
void list_init(void) {
    init();
}
#endif
