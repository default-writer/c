/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 23, 2025 at 4:21:23 PM GMT+3
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

#define USING_PY_API

#ifndef PY_API_H
#define PY_API_H

#include "py_export.h"

#include "macros.h"

#include "system/error/error_v1.h"

#include "virtual/api/api_v1.h"

PY_EXPORT extern const virtual_vm_methods* PY_PUBLIC_API(vm);
PY_EXPORT extern const virtual_methods* PY_PUBLIC_API(virtual);
PY_EXPORT extern const virtual_pointer_methods* PY_PUBLIC_API(pointer);
PY_EXPORT extern const virtual_env_methods* PY_PUBLIC_API(env);
PY_EXPORT extern const virtual_data_methods* PY_PUBLIC_API(data);
PY_EXPORT extern const virtual_file_methods* PY_PUBLIC_API(file);
PY_EXPORT extern const virtual_object_methods* PY_PUBLIC_API(object);
PY_EXPORT extern const virtual_stack_methods* PY_PUBLIC_API(stack);
PY_EXPORT extern const virtual_string_methods* PY_PUBLIC_API(string);
PY_EXPORT extern const virtual_user_methods* PY_PUBLIC_API(user);
PY_EXPORT extern const virtual_list_methods* PY_PUBLIC_API(list);

#endif // PY_API_H
