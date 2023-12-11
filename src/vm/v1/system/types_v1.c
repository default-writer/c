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

#include "types_v1.h"

/* public */
const struct types_v1 types_definition_v1 = {
    .data = &data_methods_definition_v1,
    .list = &list_methods_definition_v1,
    .file = &file_methods_definition_v1,
    .object = &object_methods_definition_v1,
    .string = &string_methods_definition_v1,
    .string_pointer = &string_pointer_methods_definition_v1,
    .user = &user_methods_definition_v1
};
