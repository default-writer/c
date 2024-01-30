/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   12 December 2023 at 8:34:56 GMT+3
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

#include "std/api.h"

#include "vm/v1/types_v1.h"

/* definition */
extern const struct list_v1 list_definition_v1;
extern const struct vm_options_v1 vm_options_definition_v1;
extern const struct os_methods_v1 os_methods_definition_v1;
extern const struct pointer_methods_v1 pointer_methods_definition_v1;
extern const struct info_v1 API(info_definition);
extern const type_methods API(type_methods_definition);
extern const struct virtual_methods_v1 virtual_methods_definition_v1;

/* definition */
#if defined(INLINE)
const const struct list_v1* list_v1 = &list_definition_v1;
const struct vm_options_v1* vm_options_v1 = &vm_options_definition_v1;
const struct os_methods_v1* os_v1 = &os_methods_definition_v1;
const struct pointer_methods_v1* pointer_v1 = &pointer_methods_definition_v1;
const struct info_v1* info_v1 = &API(info_definition);
const struct types_v1* types_v1 = &API(types_definition);
const struct virtual_methods_v1* virtual_v1 = &virtual_methods_definition_v1;
#else
/* definition */
static const struct list_v1* list_v1 = &list_definition_v1;
static const struct vm_options_v1* vm_options_v1 = &vm_options_definition_v1;
static const struct os_methods_v1* os_v1 = &os_methods_definition_v1;
static const struct pointer_methods_v1* pointer_v1 = &pointer_methods_definition_v1;
static const struct info_v1* info_v1 = &API(info_definition);
static const type_methods* types_v1 = &API(type_methods_definition);
static const struct virtual_methods_v1* virtual_v1 = &virtual_methods_definition_v1;
#endif

/* definition */
extern const struct data_methods_v1 data_methods_definition_v1;
extern const struct file_methods_v1 file_methods_definition_v1;
extern const struct list_methods_v1 list_methods_definition_v1;
extern const object_methods object_methods_definition_v1;
extern const string_methods string_methods_definition_v1;
extern const struct string_pointer_methods_v1 string_pointer_methods_definition_v1;
extern const struct user_methods_v1 user_methods_definition_v1;

/* definition */
#if defined(INLINE)
const struct data_methods_v1* type_data = &data_methods_definition_v1;
const struct file_methods_v1* type_file = &file_methods_definition_v1;
const struct list_methods_v1* type_list = &list_methods_definition_v1;
const struct object_methods* type_object = &object_methods_definition_v1;
const struct string_methods* type_string = &string_methods_definition_v1;
const struct string_pointer_methods_v1* type_string_pointer = &string_pointer_methods_definition_v1;
const struct user_methods_v1* type_user = &user_methods_definition_v1;
#else
static const struct data_methods_v1* type_data_v1 = &data_methods_definition_v1;
static const struct file_methods_v1* type_file_v1 = &file_methods_definition_v1;
static const struct list_methods_v1* type_list_v1 = &list_methods_definition_v1;
static const object_methods* type_object_v1 = &object_methods_definition_v1;
static const string_methods* type_string_v1 = &string_methods_definition_v1;
static const struct string_pointer_methods_v1* type_string_pointer_v1 = &string_pointer_methods_definition_v1;
static const struct user_methods_v1* type_user_v1 = &user_methods_definition_v1;
#endif

#endif /* _VM_V1_H_ */
