/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 25, 2025 at 2:51:02 PM GMT+3
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

#ifndef _STD_API_H_
#define _STD_API_H_

#define USING_API

#define USING_DATA
#define USING_HEADERS
#define USING_MACROS

#include "std/data.h"
#include "std/headers.h"
#include "std/macros.h"

#include "std/version.h"

#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define PATCH_VERSION 0

#define VM_EXPAND(X) X
#define VM_CONCAT_(A, B) A##B
#define VM_CONCAT(A, B) VM_CONCAT_(A, B)
#define VM_STRINGIFY_(X) #X
#define VM_STRINGIFY(X) VM_STRINGIFY_(X)

#define PRIVATE_API(x) VM_CONCAT(API(x), VM_CONCAT(_, GIT_COMMIT_HASH_VALUE))
#define API_VERSION VM_EXPAND(VM_CONCAT(, VM_STRINGIFY(VM_CONCAT(MAJOR_VERSION, VM_CONCAT(., VM_CONCAT(MINOR_VERSION, VM_CONCAT(., PATCH_VERSION)))))))
#define API(x) VM_CONCAT(x, VM_CONCAT(_, VM_CONCAT(v, VM_CONCAT(MAJOR_VERSION, VM_CONCAT(_, VM_CONCAT(MINOR_VERSION, VM_CONCAT(_, PATCH_VERSION)))))))

// #if !defined(_WIN32)
// #define CALL(x) (x)
// #else
#define CALL(x) PRIVATE_API(x)()
// #endif

#endif /* _STD_API_H_ */
