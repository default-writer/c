/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 8, 2025 at 5:55:56 PM GMT+3
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

#ifndef _POINTER_TYPES_USER_TYPE_H_
#define _POINTER_TYPES_USER_TYPE_H_

#define USING_API

#include "std/api.h"

/*! @file user_v1.h
 *  @brief C API / user
 */

typedef struct PRIVATE_API(user_methods) {
    u64 (*alloc)(void);
    void (*free)(u64 ptr);
} user_methods;

/* definition */
extern const user_methods PRIVATE_API(user_methods_definitions);

/* definition */
#ifdef INLINE
const user_methods* user = &PRIVATE_API(user_methods_definitions);
#else
/* definition */
static const user_methods* user = &PRIVATE_API(user_methods_definitions);
#endif

#endif /* _POINTER_TYPES_USER_TYPE_H_ */
