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

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "std/common.h"

#include "playground/memory/api/v1/memory_v1.h"
#include "playground/memory/api/v2/memory_v2.h"
#include "playground/memory/api/v3/memory_v3.h"

struct memory_allocator_v1;
struct memory_allocator_v2;
struct memory_allocator_v3;

union memory_allocator_api {
    struct memory_allocator_v1 v1;
    struct memory_allocator_v2 v2;
    struct memory_allocator_v3 v3;
};

#endif /* _MEMORY_H_ */
