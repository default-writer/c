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

#ifndef _POINTER_TYPES_FILE_H_
#define _POINTER_TYPES_FILE_H_

#include "std/common.h"

/*!
    @file file_v1.h
    @brief C API / file
    @image latex global-distribution.png width=5cm

    ```
    struct file_methods {
        u64 (*alloc)(u64 file_path_ptr, u64 mode_ptr);
        void (*free)(u64 ptr);
        u64 (*data)(u64 ptr);
    };
    ```
*/
struct file_methods {
    u64 (*alloc)(u64 file_path_ptr, u64 mode_ptr);
    void (*free)(u64 ptr);
    u64 (*data)(u64 ptr);
};

/* definition */
extern const struct file_methods file_methods_definition;

/* definition */
#if defined(INLINE)
const struct file_methods* file = &file_methods_definition;
#else
static const struct file_methods* file = &file_methods_definition;
#endif

#endif /* _POINTER_TYPES_FILE_H_ */
