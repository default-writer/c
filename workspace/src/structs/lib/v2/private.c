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

#include "../../common/v2/public.h"
#include "../../common/v2/type.h"

struct private_B { /* private */
    u64 counter_b;
};

struct B {
    struct public_B public;
    struct private_B private;
};

struct typeinfo ti = {
    .size = sizeof(struct B),
#ifdef USE_MEMORY_DEBUG_INFO
    .name = "B"
#endif
};

static struct typeinfo* B_typeinfo = &ti;

static void b_methods_set_counter_b(struct B* ptr, u64 value);
static u64 b_methods_get_counter_b(const struct B* ptr);
static struct typeinfo* b_methods_type(void);

static void b_methods_set_counter_b(struct B* ptr, u64 value) {
    ptr->private.counter_b = value;
}

static u64 b_methods_get_counter_b(const struct B* ptr) {
    return ptr->private.counter_b;
}

static struct typeinfo* b_methods_type(void) {
    return B_typeinfo;
}

const struct B_methods B_methods_definition = {
    .set_counter_b = b_methods_set_counter_b,
    .get_counter_b = b_methods_get_counter_b,
    .type = b_methods_type
};
