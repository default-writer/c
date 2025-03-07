/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 7, 2025 at 2:33:17 PM GMT+3
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

#ifndef _STD_DATA_H_
#define _STD_DATA_H_

#define USING_DATA

typedef unsigned long long u64;
typedef unsigned long int u32;
typedef unsigned short int u16;
typedef unsigned char u8;

typedef signed long long s64;
typedef signed long int s32;
typedef signed short int s16;
typedef signed char s8;

typedef struct stack_element* stack_ptr;
typedef struct stack_element {
    /* points to next node */
    stack_ptr next;
    /* data */
    void* data;
} stack_element;

typedef struct pointer* pointer_ptr;
typedef struct public_pointer* public_pointer_ptr;
typedef const struct pointer* const_pointer_ptr;
typedef struct virtual_pointer* virtual_pointer_ptr;
typedef const struct virtual_pointer* const_virtual_pointer_ptr;
typedef void (*desctructor)(const_pointer_ptr const_ptr);
typedef union {
    const_pointer_ptr const_ptr;
    union {
        pointer_ptr ptr;
        public_pointer_ptr pointer;
    } public;
} safe_pointer_ptr;
typedef struct type_methods_definitions {
    desctructor desctructor;
} type_methods_definitions;

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
    /* value used for stack type - stack */
    TYPE_STACK = 5,
    /* value used for object type - object */
    TYPE_OBJECT = 6,
    /* value used for user type - user (id: +0, +1, +2, +3, ...) */
    TYPE_USER = 7,
};

#endif /* _STD_DATA_H_ */
