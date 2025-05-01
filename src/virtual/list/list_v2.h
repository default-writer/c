/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 1, 2025 at 9:01:14 PM GMT+3
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

#define USING_VIRTUAL_LIST_V2

#ifndef VIRTUAL_LIST_V2_H
#define VIRTUAL_LIST_V2_H

#define USING_STD_API
#include "std/api.h"

#include "std/data_v2.h"

#include "virtual/platform.h"

typedef struct PRIVATE_API(virtual_list_v2_methods) {
    stack_v2_ptr (*init)(u64 size, stack_v2_ptr stack);
    u64 (*push)(stack_v2_ptr* current, void_ptr item);
    void_ptr (*pop)(stack_v2_ptr* current);
    void_ptr (*peek)(stack_v2_ptr* current);
    u64 (*diff)(stack_v2_ptr current1, stack_v2_ptr current2, stack_v2_ptr* compare);
    u64 (*diff_left)(stack_v2_ptr stack1, stack_v2_ptr stack2, stack_v2_ptr* compare);
    u64 (*diff_right)(stack_v2_ptr stack1, stack_v2_ptr stack2, stack_v2_ptr* compare);
    u64 (*destroy)(stack_v2_ptr* current);
} virtual_list_v2_methods;

/* definition */
CVM_EXPORT extern const virtual_list_v2_methods* CALL(list_v2);

#endif /* VIRTUAL_LIST_V2_H */
