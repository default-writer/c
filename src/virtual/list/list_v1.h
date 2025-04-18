/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 15, 2025 at 11:33:12 PM GMT+3
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

#define USING_LIST

#ifndef _VIRTUAL_LIST_H_
#define _VIRTUAL_LIST_H_

#define USING_STD_API
#include "std/api.h"

#include "virtual/export.h"

typedef struct PRIVATE_API(virtual_list_methods) {
    stack_ptr (*init)(const_vm_ptr cvm);
    u64 (*push)(const_vm_ptr cvm, stack_ptr current, void_ptr item);
    void_ptr (*pop)(const_vm_ptr cvm, stack_ptr current);
    void_ptr (*peek)(const_vm_ptr cvm, stack_ptr current);
    u64 (*diff)(const_vm_ptr cvm, stack_ptr current1, stack_ptr current2, stack_ptr result);
    u64 (*diff_left)(const_vm_ptr cvm, stack_ptr stack1, stack_ptr stack2, stack_ptr cmp);
    u64 (*diff_right)(const_vm_ptr cvm, stack_ptr stack1, stack_ptr stack2, stack_ptr cmp);
    u64 (*destroy)(const_vm_ptr cvm, stack_ptr current);
#ifdef USE_MEMORY_DEBUG_INFO
    void (*print_head)(stack_ptr current);
    void (*print)(stack_ptr current);
#endif
} virtual_list_methods;

/* definition */
CVM_EXPORT extern const virtual_list_methods* CALL(list);

#endif /* _VIRTUAL_LIST_H_ */
