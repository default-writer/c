/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   12 December 2023 at 8:38:17 GMT+3
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

#ifndef _POINTER_TYPES_DATA_H_
#define _POINTER_TYPES_DATA_H_

#include "std/data.h"

/*! @file data_v1.h
 *  @brief C API / data
 */

struct data_methods_v1 {
    u64 (*alloc)(u64 size);
    void (*free)(u64 ptr);
    void* (*unsafe)(u64 ptr);
    u64 (*size)(u64 ptr);
};

#endif /* _POINTER_TYPES_DATA_H_ */
