/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 9, 2025 at 11:03:56 AM GMT+3
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

#ifndef _VIRTUAL_STRING_V1_H_
#define _VIRTUAL_STRING_V1_H_

#define USING_STD_API
#include "std/api.h"

#include "virtual/export.h"

/*! @file string_v1.h
 *  @brief C API / string
 */

typedef struct API(virtual_string_methods) {
    u64 (*free)(const_vm_ptr cvm, u64 ptr);
    u64 (*copy)(const_vm_ptr cvm, u64 ptr);
    u64 (*strcpy)(const_vm_ptr cvm, u64 dest, u64 src);
    u64 (*strcat)(const_vm_ptr cvm, u64 dest, u64 src);
    u64 (*strrchr)(const_vm_ptr cvm, u64 src_ptr, u64 match_prt);
    u64 (*strchr)(const_vm_ptr cvm, u64 src_ptr, u64 match_prt);
    u64 (*match)(const_vm_ptr cvm, u64 src_ptr, u64 match_prt);
    u64 (*offset)(const_vm_ptr cvm, u64 src_ptr, u64 match_prt);
    u64 (*load)(const_vm_ptr cvm, const char* data);
    u64 (*put_char)(const_vm_ptr cvm, u64 ptr, char value);
    char* (*unsafe)(const_vm_ptr cvm, u64 ptr);
    u64 (*size)(const_vm_ptr cvm, u64 ptr);
    u64 (*lessthan)(const_vm_ptr cvm, u64 src, u64 dest);
    u64 (*greaterthan)(const_vm_ptr cvm, u64 src, u64 dest);
    u64 (*equals)(const_vm_ptr cvm, u64 src, u64 dest);
    u64 (*compare)(const_vm_ptr cvm, u64 src, u64 dest);
    u64 (*left)(const_vm_ptr cvm, u64 src, u64 offset);
    u64 (*strncpy)(const_vm_ptr cvm, u64 src, u64 nbytes);
    u64 (*left_strncpy)(const_vm_ptr cvm, u64 src, u64 offset);
    u64 (*right)(const_vm_ptr cvm, u64 src, u64 offset);
    u64 (*move_left)(const_vm_ptr cvm, u64 src, u64 nbytes);
    u64 (*move_right)(const_vm_ptr cvm, u64 src, u64 nbytes);
    u64 (*strcmp)(const_vm_ptr cvm, u64 src, u64 dest);
} virtual_string_methods;

/* definition */
CVM_EXPORT extern const virtual_string_methods* CALL(string);

#endif /* _VIRTUAL_STRING_V1_H_ */
