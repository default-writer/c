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

#ifndef _POINTER_TYPES_OS_H_
#define _POINTER_TYPES_OS_H_

#include "std/common.h"

/*! @file os_v1.h
 *  @brief C API / os
 */

struct os_methods {
    u64 (*getenv)(u64 name);
    u64 (*getcwd)(void);
    void (*putc)(u64 ptr);
};

/* definition */
extern const struct os_methods os_methods_definition;

/* definition */
#if defined(INLINE)
const struct os_methods* os = &os_methods_definition;
#else
static const struct os_methods* os = &os_methods_definition;
#endif

#endif /* _POINTER_TYPES_OS_H_ */
