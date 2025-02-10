/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 10, 2025 at 5:22:14 PM GMT+3
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

#include "vm/types/object/object_v1.h"
#include "vm/types/string/string_v1.h"
#include "vm/types/string_pointer/string_pointer_v1.h"
#include "vm/types/user/user_v1.h"

#include "std/api.h"

/* definition */
extern const object_methods PRIVATE_API(object_methods_definitions);
extern const string_methods PRIVATE_API(string_methods_definitions);
extern const string_pointer_methods PRIVATE_API(string_pointer_methods_definitions);
extern const user_methods PRIVATE_API(user_methods_definitions);

/* definition */
#ifdef INLINE
const object_methods* virtual_object = &PRIVATE_API(object_methods_definitions);
const string_methods* virtual_string = &PRIVATE_API(string_methods_definitions);
const string_pointer_methods* virtual_string_pointer = &PRIVATE_API(string_pointer_methods_definitions);
const user_methods* virtual_user = &PRIVATE_API(user_methods_definitions);
#else
/* definition */
static const object_methods* virtual_object = &PRIVATE_API(object_methods_definitions);
static const string_methods* virtual_string = &PRIVATE_API(string_methods_definitions);
static const string_pointer_methods* virtual_string_pointer = &PRIVATE_API(string_pointer_methods_definitions);
static const user_methods* virtual_user = &PRIVATE_API(user_methods_definitions);
#endif

#endif /* _VM_V1_H_ */
