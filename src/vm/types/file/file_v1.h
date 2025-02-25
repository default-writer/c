/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 25, 2025 at 2:49:02 PM GMT+3
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

#ifndef _FILE_V1_H_
#define _FILE_V1_H_

#define USING_API

#include "std/api.h"

#include "vm/export.h"

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

typedef struct PRIVATE_API(virtual_file_methods) {
    u64 (*alloc)(u64 file_path_ptr, u64 mode_ptr);
    void (*free)(u64 ptr);
    u64 (*data)(u64 ptr);
} virtual_file_methods;

/* definition */
CVM_EXPORT extern const virtual_file_methods PRIVATE_API(virtual_file_methods_definitions);
CVM_EXPORT extern const virtual_file_methods* CALL(virtual_file);

#endif /* _FILE_V1_H_ */
