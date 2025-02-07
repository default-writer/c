/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 7, 2025 at 8:40:17 AM GMT+3
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

#include "user_v1.h"

#include "std/api.h"

#include "vm/v1/pointer/pointer_v1.h"
#include "vm/v1/virtual/virtual_v1.h"
#include "vm/v1/vm_type.h"
#include "vm/vm_type.h"

#define DEFAULT_SIZE 0x100

static u64 id = TYPE_NULL;

#ifndef ATTRIBUTE
void user_virtual_init(void);
#endif

/* internal */
static u64 user_alloc(void);
static void user_free(u64 ptr);

/* destructor */
static void virtual_free(struct pointer* ptr);

/* implementation */
static u64 user_alloc(void) {
    struct pointer* ptr = pointer->alloc(0, id);
    u64 virtual_ptr = virtual->alloc(ptr);
    return virtual_ptr;
}

static void user_free(u64 ptr) {
    struct pointer* data_ptr = virtual->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    virtual_free(data_ptr);
}

static void virtual_free(struct pointer* ptr) {
    pointer->release(ptr);
}

static const struct vm_type _type = {
    .free = virtual_free
};

static void INIT init(void) {
    id = pointer->register_type(TYPE_NULL, &_type);
}

/* public */
const user_methods PRIVATE_API(user_methods_definition) = {
    .alloc = user_alloc,
    .free = user_free
};

#ifndef ATTRIBUTE
void user_virtual_init(void) {
    init();
}
#endif
