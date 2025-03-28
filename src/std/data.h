/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 9:43:53 AM GMT+3
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
typedef const struct stack_element* const_stack_element_ptr;
typedef struct stack_element {
    stack_ptr next;
    void* data;
} stack_element_type;
typedef struct pointer* pointer_ptr;
typedef const struct pointer* const_pointer_ptr;
typedef struct virtual_pointer* virtual_pointer_ptr;
typedef const struct virtual_pointer* const_virtual_pointer_ptr;
typedef struct address* address_ptr;
typedef struct address {
    u64 address;
    const_pointer_ptr ptr;
} address_type;
typedef union {
    const_pointer_ptr const_ptr;
    pointer_ptr ptr;
} safe_pointer_ptr;
typedef struct vm* vm_ptr;
typedef const vm_ptr* const_vm_ptr;
typedef void* (*type_constructor)(u64 size);
typedef void (*type_destructor)(u64 address);
typedef void (*type_free)(const_pointer_ptr const_ptr, u64 address);
typedef union {
    const_vm_ptr const_ptr;
    vm_ptr* ptr;
} safe_vm_ptr;
typedef const void* const_void_ptr;
typedef union {
    const_void_ptr const_ptr;
    void* ptr;
} safe_void_ptr;
typedef struct type_methods_definitions* type_methods_definitions_ptr;
typedef const struct type_methods_definitions* const_type_methods_definitions_ptr;
typedef union {
    const_type_methods_definitions_ptr const_ptr;
    type_methods_definitions_ptr ptr;
} safe_type_methods_definitions;
typedef struct type_methods_definitions {
    u64 type_id;
    type_constructor constructor;
    type_destructor destructor;
    type_free free;
} type_methods_definitions_type;
typedef struct pointer_public* pointer_public_ptr;
typedef const struct pointer_public* const_pointer_public_ptr;
typedef struct pointer_public {
    u64 address;
    u64 size;
    u64 type;
} pointer_public_type;
typedef struct hashentry* hashentry_ptr;
typedef const struct hashentry* const_hashentry_ptr;
typedef struct hashentry {
    u64 key;
    const_void_ptr value;
    hashentry_ptr next;
} hashentry_type;
typedef struct hashtable* hashtable_ptr;
typedef const struct hashtable* const_hashtable_ptr;
typedef struct hashtable {
    hashentry_ptr* table;
    u64 capacity;
    u64 size;
} hashtable_type;

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
