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

#include "../../common/v4/public.h"
#include "../../common/v4/type.h"

struct private_B { /* private */
    u64 counter_b;
};

struct B {
    struct public_B public;
    struct private_B private;
};

static const struct public_B** B_enter(struct object* ptr);
static void B_leave(const struct public_B** ptr);

static void B_create_object(struct B* ptr);
static void B_destroy_object(struct B* ptr);

struct typeinfo ti = {
    .size = sizeof(struct B),
#ifdef USE_MEMORY_DEBUG_INFO
    .name = "B"
#endif
};

struct methodinfo mi = {
    .create = B_create_object,
    .destroy = B_destroy_object
};

static struct typeinfo* B_typeinfo = &ti;
static struct methodinfo* B_methodinfo = &mi;

static void b_set_counter_b(u64 value);
static u64 b_get_counter_b();
static struct typeinfo* b_type(void);
static struct methodinfo* b_methodinfo(void);

static struct B* this;
static const struct public_B** public = (const struct public_B**)&this;

static const struct public_B** B_enter(struct object* ptr) {
    this = (struct B*)ptr;
    return public;
}

static void B_leave(const struct public_B** ptr) {
    this = 0;
}

static void B_create_object(struct B* ptr) {
    ptr->public.get_counter_b = b_get_counter_b;
    ptr->public.set_counter_b = b_set_counter_b;
}

static void B_destroy_object(struct B* ptr) {
    ptr->public.get_counter_b = 0;
    ptr->public.set_counter_b = 0;
}

static u64 b_get_counter_b() {
    return this->private.counter_b;
}

static void b_set_counter_b(u64 value) {
    this->private.counter_b = value;
}

static struct typeinfo* b_type(void) {
    return B_typeinfo;
}

static struct methodinfo* b_method(void) {
    return B_methodinfo;
}

const struct class B_class_definition = {
    .type = b_type,
    .method = b_method,
    .enter = B_enter,
    .leave = B_leave
};
