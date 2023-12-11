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

#ifndef _B_H_
#define _B_H_

#include "class/class.h"
#include "common/memory.h"
#include "std/common.h"

#include "A.h"

typedef struct B {
    const A a;
    const u64 counter;
} const* B;

typedef struct interface_B struct_interface_B;
typedef struct_interface_B* interface_B;

struct interface_B {
    object_typeinfo (*initialize)(void);
    void (*print)(object_typeinfo ptr);
    void (*finalize)(object_typeinfo ptr);
};

#endif /* _B_H_ */
