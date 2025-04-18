/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 15, 2025 at 9:17:06 PM GMT+3
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

#define USING_SYSTEM_ERROR

#ifndef _SYSTEM_ERROR_H_
#define _SYSTEM_ERROR_H_

#define USING_STD_API
#include "std/api.h"

#include "system/export.h"

#include <stdio.h>

enum error_message_code {
    ID_ERROR_NO_ERROR,
    ID_ERROR_VM_NOT_INITIALIZED,
    ID_ERROR_INVALID_POINTER,
    ID_ERROR_INVALID_ARGUMENT,
    ID_ERROR_INVALID_TYPE_ID,
    ID_ERROR_INVALID_VALUE
};

typedef struct PRIVATE_API(system_error_methods) {
    FILE* (*stdout)(void);
    FILE* (*stderr)(void);
    void (*output)(FILE* output, u64 error_type, const char* message, u64 size);
    void (*throw)(u64 error_type, const char* message, u64 size);
    void (*clear)(void);
    u64 (*type)(void);
    const char* (*get)(void);
    u64 (*next)(void);
    u64 (*count)(void);
} system_error_methods;

/* public */
CSYS_EXPORT extern const system_error_methods* CALL(error);

#endif /* _SYSTEM_ERROR_H_ */
