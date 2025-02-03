/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 3, 2025 at 9:06:49 PM GMT+3
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

#ifndef _VM_V1_H_
#define _VM_V1_H_

#define USING_VM_V1
#define USING_TYPES_V1

#include "std/api.h"

#include "vm/v1/types_v1.h"

/* definition */
extern const object_methods PRIVATE_API(object_methods_definition);
extern const string_methods PRIVATE_API(string_methods_definition);
extern const string_pointer_methods PRIVATE_API(string_pointer_methods_definition);
extern const user_methods PRIVATE_API(user_methods_definition);

/* definition */
#ifdef INLINE
const object_methods* type_object_v1 = &PRIVATE_API(object_methods_definition);
const string_methods* type_string_v1 = &PRIVATE_API(string_methods_definition);
const string_pointer_methods* type_string_pointer_v1 = &PRIVATE_API(string_pointer_methods_definition);
const user_methods* type_user_v1 = &PRIVATE_API(user_methods_definition);
#else
/* definition */
static const object_methods* type_object_v1 = &PRIVATE_API(object_methods_definition);
static const string_methods* type_string_v1 = &PRIVATE_API(string_methods_definition);
static const string_pointer_methods* type_string_pointer_v1 = &PRIVATE_API(string_pointer_methods_definition);
static const user_methods* type_user_v1 = &PRIVATE_API(user_methods_definition);
#endif

#endif /* _VM_V1_H_ */
