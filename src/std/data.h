/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 9, 2025 at 4:01:58 PM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _STD_DATA_H_
#define _STD_DATA_H_

#define USING_DATA
#define VM_PTR_ARRAY_SIZE 4096
#define ERROR_BUFFER_SIZE 4096
#define ERROR_MESSAGE_SIZE 256
#define ERROR_MESSAGE_COUNT (ERROR_BUFFER_SIZE / ERROR_MESSAGE_SIZE)

typedef unsigned long long u64;
typedef unsigned long int u32;
typedef unsigned short int u16;
typedef unsigned char u8;

typedef signed long long s64;
typedef signed long int s32;
typedef signed short int s16;
typedef signed char s8;

typedef struct virtual_pointer* virtual_pointer_ptr;
typedef const struct virtual_pointer* const_virtual_pointer_ptr;

typedef struct vm* vm_ptr;
typedef const struct vm** const_vm_ptr;
typedef struct pointer* pointer_ptr;
typedef const struct pointer* const_pointer_ptr;

typedef struct type_methods_definitions* type_methods_definitions_ptr;
typedef const struct type_methods_definitions* const_type_methods_definitions_ptr;

typedef struct known_types* known_types_ptr;
typedef const struct known_types* const_known_types_ptr;
typedef struct pointer_public* pointer_public_ptr;
typedef const struct pointer_public* const_pointer_public_ptr;

typedef struct stack* stack_ptr;
typedef const struct stack* const_stack_ptr;
typedef struct stack_element* stack_element_ptr;
typedef const struct stack_element* const_stack_element_ptr;
typedef struct hashentry* hashentry_ptr;
typedef const struct hashentry* const_hashentry_ptr;
typedef struct hashtable* hashtable_ptr;
typedef const struct hashtable* const_hashtable_ptr;
typedef struct exception* exception_ptr;
typedef const struct exception* const_exception_ptr;

typedef void* void_ptr;
typedef const void* const_void_ptr;

typedef void_ptr (*type_constructor)(u64 size);
typedef void (*type_destructor)(const_vm_ptr cvm, u64 address);

typedef struct type_methods_definitions {
    u64 type_id;
    type_constructor constructor;
    type_destructor destructor;
} type_methods_definitions_type;
typedef struct known_types {
    type_methods_definitions_ptr methods;
} known_types_type;
typedef struct pointer_public {
    u64 address;
    u64 size;
    u64 type;
} pointer_public_type;

typedef struct stack {
    stack_element_ptr current;
    u64 size;
} stack_type;
typedef struct stack_element {
    stack_element_ptr next;
    void_ptr data;
} stack_element_type;
typedef struct hashentry {
    u64 key;
    const_void_ptr value;
    hashentry_ptr next;
} hashentry_type;
typedef struct hashtable {
    hashentry_ptr* table;
    u64 capacity;
    u64 size;
} hashtable_type;
typedef struct exception {
    u64 message_count;
    u64 type[ERROR_MESSAGE_COUNT];
    char message[ERROR_BUFFER_SIZE];
} exception_type;

typedef union {
    const_vm_ptr const_ptr;
    vm_ptr* ptr;
} safe_vm_ptr;
typedef union {
    const_pointer_ptr const_ptr;
    pointer_ptr ptr;
} safe_pointer_ptr;
typedef union {
    const_type_methods_definitions_ptr const_ptr;
    type_methods_definitions_ptr ptr;
} safe_type_methods_definitions;
typedef union {
    const virtual_pointer_ptr* const_ptr;
    virtual_pointer_ptr* ptr;
} safe_virtual_pointer_ptr;

typedef union {
    const_void_ptr const_ptr;
    void_ptr ptr;
} safe_void_ptr;

enum type {
    /* value used for ephemeral type - null */
    TYPE_NULL = 0,
    /* value used for pointer type - ref */
    TYPE_DATA = 1,
    /* value used for file type - file */
    TYPE_FILE = 2,
    /* value used for object type - object */
    TYPE_OBJECT = 3,
    /* value used for stack type - stack */
    TYPE_STACK = 4,
    /* value used for string type - string */
    TYPE_STRING = 5,
    /* value used for string ref type - string ref */
    TYPE_STRING_POINTER = 6,
    /* value used for user type - user (id: +0, +1, +2, +3, ...) */
    TYPE_USER = 7,
};

#endif /* _STD_DATA_H_ */
