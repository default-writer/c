/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 6, 2025 at 9:34:30 AM GMT+3
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

#include "virtual/pointer/pointer_v1.h"
#include "virtual/virtual/virtual_v1.h"

#define DEFAULT_SIZE 0x100

static u64 id = TYPE_USER;

/* internal */
static u64 virtual_user_alloc(void);
static void virtual_user_free(u64 ptr);

/* destructor */
static void type_desctructor(const_pointer_ptr ptr);

/* implementation */
static const struct type_methods_definitions user_type = {
    .desctructor = type_desctructor
};

static void INIT init(void) {
    id = CALL(pointer)->register_user_type(&user_type);
}

static void type_desctructor(const_pointer_ptr ptr) {
    CALL(pointer)->release(ptr);
}

static u64 virtual_user_alloc(void) {
    return CALL(virtual)->pointer(0, id);
}

static void virtual_user_free(u64 ptr) {
    const_pointer_ptr data_ptr = CALL(virtual)->read_type(ptr, id);
    if (data_ptr == 0) {
        return;
    }
    type_desctructor(data_ptr);
}

#ifndef ATTRIBUTE
void user_init(void) {
    init();
}
#endif

/* public */
const virtual_user_methods PRIVATE_API(virtual_user_methods_definitions) = {
    .alloc = virtual_user_alloc,
    .free = virtual_user_free
};

const virtual_user_methods* user = &PRIVATE_API(virtual_user_methods_definitions);

const virtual_user_methods* CALL(user) {
    return user;
}
