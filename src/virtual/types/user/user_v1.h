/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 6, 2025 at 12:34:07 AM GMT+3
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

#ifndef _USER_V1_H_
#define _USER_V1_H_

#define USING_API

#include "std/api.h"

#include "vm/export.h"

/*! @file virtual_user_v1.h
 *  @brief C API / virtual_user
 */

typedef struct PRIVATE_API(virtual_user_methods) {
    u64 (*alloc)(void);
    void (*free)(u64 ptr);
} virtual_user_methods;

/* definition */
CVM_EXPORT extern const virtual_user_methods* CALL(user);

#endif /* _USER_V1_H_ */
