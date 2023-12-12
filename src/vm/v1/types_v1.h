/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   12 December 2023 at 8:36:28 GMT+3
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
#ifndef _POINTER_TYPES_V1_H_
#define _POINTER_TYPES_V1_H_

#include "vm/v1/types/data/data_v1.h"
#include "vm/v1/types/file/file_v1.h"
#include "vm/v1/types/list/list_v1.h"
#include "vm/v1/types/object/object_v1.h"
#include "vm/v1/types/string/string_v1.h"
#include "vm/v1/types/string_pointer/string_pointer_v1.h"
#include "vm/v1/types/user/user_v1.h"

typedef struct API(type_methods) type_methods;

struct API(type_methods) {
    const struct data_methods_v1* data;
    const struct file_methods_v1* file;
    const struct list_methods_v1* list;
    const object_methods* object;
    const string_methods* string;
    const struct string_pointer_methods_v1* string_pointer;
    const struct user_methods_v1* user;
};

#endif /* _POINTER_TYPES_V1_H_ */
