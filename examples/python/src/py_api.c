/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 29, 2025 at 4:33:24 PM GMT+3
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

#include "py_api.h"

#include "system/error/error_v1.h"

/* api */
PUBLIC const virtual_vm_methods* PY_PUBLIC_API(vm) {
    CALL(error)->clear();
    return PUBLIC_API(vm);
}

PUBLIC const virtual_methods* PY_PUBLIC_API(virtual) {
    CALL(error)->clear();
    return PUBLIC_API(virtual);
}

PUBLIC const virtual_pointer_methods* PY_PUBLIC_API(pointer) {
    CALL(error)->clear();
    return PUBLIC_API(pointer);
}

PUBLIC const virtual_env_methods* PY_PUBLIC_API(env) {
    CALL(error)->clear();
    return PUBLIC_API(env);
}

PUBLIC const virtual_data_methods* PY_PUBLIC_API(data) {
    CALL(error)->clear();
    return PUBLIC_API(data);
}

PUBLIC const virtual_file_methods* PY_PUBLIC_API(file) {
    CALL(error)->clear();
    return PUBLIC_API(file);
}

PUBLIC const virtual_object_methods* PY_PUBLIC_API(object) {
    CALL(error)->clear();
    return PUBLIC_API(object);
}

PUBLIC const virtual_stack_methods* PY_PUBLIC_API(stack) {
    CALL(error)->clear();
    return PUBLIC_API(stack);
}

PUBLIC const virtual_string_methods* PY_PUBLIC_API(string) {
    CALL(error)->clear();
    return PUBLIC_API(string);
}

PUBLIC const virtual_user_methods* PY_PUBLIC_API(user) {
    CALL(error)->clear();
    return PUBLIC_API(user);
}

PUBLIC const virtual_list_methods* PY_PUBLIC_API(list) {
    CALL(error)->clear();
    return PUBLIC_API(list);
}

PUBLIC const virtual_list_v2_methods* PY_PUBLIC_API(list_v2) {
    CALL(error)->clear();
    return PUBLIC_API(list_v2);
}
