/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 4, 2025 at 6:09:59 PM GMT+3
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

#ifndef _INTERNAL_STRING_REFERENCE_TYPE_V1_H_
#define _INTERNAL_STRING_REFERENCE_TYPE_V1_H_

#define USING_STD_API
#include "std/api.h"

typedef struct string_reference* string_reference_ptr;
typedef const struct string_reference* const_string_reference_ptr;
typedef struct string_reference {
    u64 address;
    u64 offset;
} string_reference_type;

#endif /* _INTERNAL_STRING_REFERENCE_TYPE_V1_H_ */
