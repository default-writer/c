/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 6, 2025 at 8:30:30 AM GMT+3
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

#define USING_VIRTUAL_POINTER

#ifndef VIRTUAL_POINTER_V1_H
#define VIRTUAL_POINTER_V1_H

#define USING_STD_API
#include "std/api.h"

#include "virtual/platform.h"

/*! @file pointer_v1.h
 *  @brief C API / pointer
 */

typedef struct PRIVATE_API(virtual_pointer_methods) {
    u64 (*alloc)(const_vm_ptr cvm, const_void_ptr data, u64 size, u64 offset, u64 flags, u64 type_id);
    u64 (*copy)(const_vm_ptr cvm, const_void_ptr src, u64 size, u64 offset, u64 type_id);
    const_void_ptr (*read)(const_vm_ptr cvm, u64 address, u64 type_id);
    u64 (*ref)(const_vm_ptr cvm, u64 address);
#ifndef USE_DYNAMIC_TYPES
    u64 (*release)(const_vm_ptr cvm, u64 address, u64 type_id);
#endif
    u64 (*free)(const_vm_ptr cvm, u64 address);
} virtual_pointer_methods;

typedef struct PRIVATE_API(virtual_type_methods) {
#ifdef USE_DYNAMIC_TYPES
    void (*register_known_type)(const_vm_ptr cvm, type_methods_definitions_ptr data_type);
    void (*register_user_type)(const_vm_ptr cvm, type_methods_definitions_ptr data_type);
#else
    void (*register_known_type)(type_methods_definitions_ptr data_type);
    void (*register_user_type)(type_methods_definitions_ptr data_type);
#endif
} virtual_type_methods;

/* definition */
CVM_EXPORT extern const virtual_pointer_methods* CALL(pointer);
CVM_EXPORT extern const virtual_type_methods* CALL(type);

#endif /* VIRTUAL_POINTER_V1_H */
