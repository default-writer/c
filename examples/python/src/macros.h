/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 23, 2025 at 2:31:33 PM GMT+3
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

#define USING_MACROS

#ifndef MACROS_H
#define MACROS_H

#include <Python.h>

#ifdef PyVarObject_HEAD_INIT
#undef PyVarObject_HEAD_INIT
#define PyVarObject_HEAD_INIT(type, size) \
    {                                     \
        PyObject_HEAD_INIT(type)(size)    \
    }
#endif

#ifdef PyObject_HEAD
#undef PyObject_HEAD
#define PyObject_HEAD PyObject ob_base
#endif

#define PYTHON_ERROR(exception, format, ...)                                                                                                            \
    do {                                                                                                                                                \
        int snp_format_size = snprintf(NULL, 0, format ": %s (%s:%d)", __VA_ARGS__, __func__, __FILE__, __LINE__); /* NOLINT: snprintf(NULL) */         \
        char snp_format_buffer[snp_format_size + 1];                                                                                                    \
        snprintf(snp_format_buffer, sizeof snp_format_buffer, format ": %s (%s:%d)", __VA_ARGS__, __func__, __FILE__, __LINE__); /* NOLINT: snprintf */ \
        PyErr_SetString(exception, &snp_format_buffer[0]);                                                                                              \
        CALL(error)->clear();                                                                                                                           \
    } while (0)

#define PY_CALL(x) PY_PUBLIC_API(x)
#define PY_PUBLIC_API(x) PUBLIC_API(py_##x)

#endif // MACROS_H
