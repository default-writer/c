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

#ifndef _TYPE_V1_H_
#define _TYPE_V1_H_

enum type {
    /* value used for ephemeral type - null */
    TYPE_NULL = 0,
    /* value used for pointer type - ref */
    TYPE_DATA = 1,
    /* value used for string type - string */
    TYPE_STRING = 2,
    /* value used for string ref type - string ref */
    TYPE_STRING_POINTER = 3,
    /* value used for file type - file */
    TYPE_FILE = 4,
    /* value used for list type - list */
    TYPE_LIST = 5,
    /* value used for object type - object */
    TYPE_OBJECT = 6,
    /* value used for user type - user (id: +0, +1, +2, +3, ...) */
    TYPE_USER = 7
};

struct pointer;

struct vm_type {
    void (*free)(struct pointer* ptr);
};

#endif /* _TYPE_V1_H_ */
