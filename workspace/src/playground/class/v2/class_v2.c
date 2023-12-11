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

#include "playground/class/v2/class_v2.h"
#include "common/memory.h"
#include "playground/list/v2/list_v2.h"

/* definition */
extern const struct list list_v2;
/* class data definition */
/* extern struct class_data class_data; */

/* class data */
struct class_data {
    /* list pointer */
    struct list_data* list;
    /* data pointer */
    void* ptr;
    /* list */
};

/* implementation */

/* private */

/* definition */
static const struct list* list = &list_v2;

/* creates the class instance */
static struct class_data* _new(void);
/* deletes the class instance */
static void _delete(struct class_data* ptr);

/* proxy for the class function get_data() */
static void* class_get(struct class_data* class);
/* proxy for the class function set_data( void*)*/
static void class_set(struct class_data* class, void* data);

/* size of a memory block to allocate */
static const size_t _size = sizeof(struct class_data);

/* allocates memory pointer */
static struct class_data* _new(void) {
    /* returns class object */
    return memory->alloc(_size);
}

/* releases memory pointer */
static void _delete(struct class_data* class) {
    /* releases the pointer */
    memory->free(class, _size);
}

/* initializes the new context's head element */
static struct class_data* class_new(struct list_data* ptr) {
    /* creates emtpy data chunk */
    struct class_data* struct_class = _new();
    struct_class->list = ptr;
    return struct_class;
}

/* destroys the memory stack */
static void class_delete(struct class_data* class) {
    /* releases the pointer */
    _delete(class);
}

static void* class_get_data(struct class_data* class) {
    return class->ptr;
}

static void class_set_data(struct class_data* class, void* data) {
    class->ptr = data;
}

static void class_push(struct class_data* class) {
    /* pushes to the list */
    list->push(class->list, class);
}

static struct class_data* class_pop(struct class_data* class) {
    /* pops from the list */
    return list->pop(class->list);
}

static void* class_get(struct class_data* class) {
    /* returns data */
    return class_get_data(list->peek(class->list));
}

static void class_set(struct class_data* class, void* data) {
    /* updates the data */
    class_set_data(list->peek(class->list), data);
}

/* public */

const struct class class_definition_v2 = {
    .new = class_new,
    .delete = class_delete,
    /* generic methods */
    .push = class_push,
    .pop = class_pop,
    .get = class_get,
    .set = class_set
};
