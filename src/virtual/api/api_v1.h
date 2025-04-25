/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 23, 2025 at 4:20:06 PM GMT+3
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

#define USING_VIRTUAL_API

#ifndef _VIRTUAL_API_
#define _VIRTUAL_API_

#define USING_STD_API
#include "std/api.h"

#include "virtual/env/env_v1.h"
#include "virtual/export.h"
#include "virtual/list/list_v1.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/data/data_v1.h"
#include "virtual/types/file/file_v1.h"
#include "virtual/types/object/object_v1.h"
#include "virtual/types/stack/stack_v1.h"
#include "virtual/types/string/string_v1.h"
#include "virtual/types/user/user_v1.h"
#include "virtual/virtual/virtual_v1.h"
#include "virtual/vm/vm_v1.h"

/* definition */
CVM_EXPORT extern const virtual_vm_methods* PUBLIC_API(vm);
CVM_EXPORT extern const virtual_methods* PUBLIC_API(virtual);
CVM_EXPORT extern const virtual_pointer_methods* PUBLIC_API(pointer);
CVM_EXPORT extern const virtual_env_methods* PUBLIC_API(env);
CVM_EXPORT extern const virtual_data_methods* PUBLIC_API(data);
CVM_EXPORT extern const virtual_file_methods* PUBLIC_API(file);
CVM_EXPORT extern const virtual_object_methods* PUBLIC_API(object);
CVM_EXPORT extern const virtual_stack_methods* PUBLIC_API(stack);
CVM_EXPORT extern const virtual_string_methods* PUBLIC_API(string);
CVM_EXPORT extern const virtual_user_methods* PUBLIC_API(user);
CVM_EXPORT extern const virtual_list_methods* PUBLIC_API(list);

#endif /* _VIRTUAL_API_ */
