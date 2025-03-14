/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 14, 2025 at 7:10:00 AM GMT+3
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

#ifndef _VIRTUAL_STRING_V1_H_
#define _VIRTUAL_STRING_V1_H_

#define USING_STD_API
#include "std/api.h"

#include "vm/export.h"

/*! @file string_v1.h
 *  @brief C API / string
 */

typedef struct API(virtual_string_methods) {
    u64 (*free)(const_vm_ptr vm, u64 ptr);
    u64 (*copy)(const_vm_ptr vm, u64 ptr);
    u64 (*strcpy)(const_vm_ptr vm, u64 dest, u64 src);
    u64 (*strcat)(const_vm_ptr vm, u64 dest, u64 src);
    u64 (*strrchr)(const_vm_ptr vm, u64 src_ptr, u64 match_prt);
    u64 (*strchr)(const_vm_ptr vm, u64 src_ptr, u64 match_prt);
    u64 (*match)(const_vm_ptr vm, u64 src_ptr, u64 match_prt);
    u64 (*offset)(const_vm_ptr vm, u64 src_ptr, u64 match_prt);
    u64 (*load)(const_vm_ptr vm, const char* data);
    u64 (*put_char)(const_vm_ptr vm, u64 ptr, char value);
    char* (*unsafe)(const_vm_ptr vm, u64 ptr);
    u64 (*size)(const_vm_ptr vm, u64 ptr);
    u64 (*lessthan)(const_vm_ptr vm, u64 src, u64 dest);
    u64 (*greaterthan)(const_vm_ptr vm, u64 src, u64 dest);
    u64 (*equals)(const_vm_ptr vm, u64 src, u64 dest);
    u64 (*compare)(const_vm_ptr vm, u64 src, u64 dest);
    u64 (*left)(const_vm_ptr vm, u64 src, u64 offset);
    u64 (*strncpy)(const_vm_ptr vm, u64 src, u64 nbytes);
    u64 (*left_strncpy)(const_vm_ptr vm, u64 src, u64 offset);
    u64 (*right)(const_vm_ptr vm, u64 src, u64 offset);
    u64 (*move_left)(const_vm_ptr vm, u64 src, u64 nbytes);
    u64 (*move_right)(const_vm_ptr vm, u64 src, u64 nbytes);
    u64 (*strcmp)(const_vm_ptr vm, u64 src, u64 dest);
} virtual_string_methods;

/* definition */
CVM_EXPORT extern const virtual_string_methods* CALL(string);

#endif /* _VIRTUAL_STRING_V1_H_ */
