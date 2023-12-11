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

#include "../../common/v3/public.h"
#include "../../common/v3/type.h"

struct private_B { /* private */
    u64 counter_b;
};

struct B {
    struct public_B public;
    struct private_B private;
};

static void B_create(struct B* ptr);
static void B_destroy(struct B* ptr);

struct typeinfo ti = {
    .size = sizeof(struct B),
#ifdef USE_MEMORY_DEBUG_INFO
    .name = "B"
#endif
};

struct methodinfo mi = {
    .create = B_create,
    .destroy = B_destroy
};

static struct typeinfo* B_typeinfo = &ti;
static struct methodinfo* B_methodinfo = &mi;

static void b_set_counter_b(struct B* ptr, u64 value);
static u64 b_get_counter_b(const struct B* ptr);
static struct typeinfo* b_type(void);
static struct methodinfo* b_methodinfo(void);

static void B_create(struct B* ptr) {
    struct B* b = (struct B*)ptr;
    b->public.get_counter_b = b_get_counter_b;
    b->public.set_counter_b = b_set_counter_b;
}

static void B_destroy(struct B* ptr) {
    struct B* b = (struct B*)ptr;
    b->public.get_counter_b = 0;
    b->public.set_counter_b = 0;
}

static void b_set_counter_b(struct B* ptr, u64 value) {
    ptr->private.counter_b = value;
}

static u64 b_get_counter_b(const struct B* ptr) {
    return ptr->private.counter_b;
}

static struct typeinfo* b_type(void) {
    return B_typeinfo;
}

static struct methodinfo* b_method(void) {
    return B_methodinfo;
}

const struct class B_class_definition = {
    .type = b_type,
    .method = b_method
};
