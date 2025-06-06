/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 1, 2025 at 9:02:12 PM GMT+3
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

#define USING_SYSTEM_API_V1

#ifndef SYSTEM_API_V1_H
#define SYSTEM_API_V1_H

#define USING_STD_API
#include "std/api.h"

#include "system/platform.h"

#include "system/error/error_v1.h"
#include "system/hashtable/hashtable_v1.h"
#include "system/info/info_v1.h"
#include "system/memory/memory_v1.h"
#include "system/options/options_v1.h"
#include "system/os/os_v1.h"

/* definition */
CSYS_EXPORT extern const system_error_methods* PUBLIC_API(error);
CSYS_EXPORT extern const system_info_methods* PUBLIC_API(info);
CSYS_EXPORT extern const system_memory_methods* PUBLIC_API(memory);
CSYS_EXPORT extern const system_options_methods* PUBLIC_API(options);
CSYS_EXPORT extern const system_os_methods* PUBLIC_API(os);
CSYS_EXPORT extern const system_hashtable_methods* PUBLIC_API(hashtable);

#endif /* SYSTEM_API_V1_H */
