/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 7, 2025 at 7:58:12 AM GMT+3
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

#include "types_v1.h"

#include "std/api.h"

/* public */
const type_methods PRIVATE_API(type_methods_definition) = {
    .data = &PRIVATE_API(data_methods_definition),
    .file = &PRIVATE_API(file_methods_definition),
    .object = &PRIVATE_API(object_methods_definition),
    .stack = &PRIVATE_API(stack_methods_definition),
    .string = &PRIVATE_API(string_methods_definition),
    .string_pointer = &PRIVATE_API(string_pointer_methods_definition),
    .user = &PRIVATE_API(user_methods_definition)
};
