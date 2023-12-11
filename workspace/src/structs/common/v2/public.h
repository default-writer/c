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

#ifndef _PUBLIC_V2_H_
#define _PUBLIC_V2_H_

#include "std/common.h"

struct A {
    u64 counter_a;
};

struct B;

struct public_B { /* public */
    struct A base;
    /* public methods */
    void (*set_counter_b)(struct B* ptr, u64 value);
};

struct typeinfo;

struct B_methods {
    void (*set_counter_b)(struct B* ptr, u64 value);
    u64 (*get_counter_b)(const struct B* ptr);
    struct typeinfo* (*type)(void);
};

extern const struct B_methods B_methods_definition;

#if defined(INLINE)
const struct B_methods* B = &B_methods_definition;
#else
static const struct B_methods* B = &B_methods_definition;
#endif

#endif /* _PUBLIC_V2_H_ */
