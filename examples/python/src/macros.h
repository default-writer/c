/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 12, 2025 at 5:36:52 AM GMT+3
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

#define VA_CAR(...) VA_CAR_IMPL(__VA_ARGS__, )
#define VA_CAR_IMPL(first, ...) first

#define VA_CDR(...) VA_CDR_IMPL(, __VA_ARGS__)
#define VA_CDR_IMPL(first, ...) __VA_ARGS__

#define PYTHON_ERROR(exception, ...)                                                                                                                                  \
    do {                                                                                                                                                              \
        const char* exception##_error_msg = CALL(error)->get();                                                                                                       \
        if (exception##_error_msg != NULL_PTR) {                                                                                                                      \
            const char* exception##_msg = VA_CAR(__VA_ARGS__) ": %s (%s): %s";                                                                                        \
            int snp_format_size = CALL(error)->print(NULL, 0, exception##_msg, __func__, __FILE__, exception##_error_msg, VA_CDR(__VA_ARGS__)); /* NOLINT */          \
            char* snp_format_buffer = (char*)calloc(1, snp_format_size + 1);                                                                                          \
            CALL(error)->print(snp_format_buffer, snp_format_size + 1, exception##_msg, __func__, __FILE__, exception##_error_msg, VA_CDR(__VA_ARGS__)); /* NOLINT */ \
            PyErr_SetString(exception, snp_format_buffer);                                                                                                            \
            free(snp_format_buffer);                                                                                                                                  \
        } else {                                                                                                                                                      \
            const char* exception##_msg = VA_CAR(__VA_ARGS__) ": %s (%s)";                                                                                            \
            int snp_format_size = CALL(error)->print(NULL, 0, exception##_msg, __func__, __FILE__, __LINE__, VA_CDR(__VA_ARGS__)); /* NOLINT */                       \
            char* snp_format_buffer = (char*)calloc(1, snp_format_size + 1);                                                                                          \
            CALL(error)->print(snp_format_buffer, snp_format_size + 1, exception##_msg, __func__, __FILE__, __LINE__, VA_CDR(__VA_ARGS__)); /* NOLINT */              \
            PyErr_SetString(exception, snp_format_buffer);                                                                                                            \
            free(snp_format_buffer);                                                                                                                                  \
        }                                                                                                                                                             \
        CALL(error)->clear();                                                                                                                                         \
    } while (0)

#define PY_CALL(x) PY_PUBLIC_API(x)
#define PY_PUBLIC_API(x) PUBLIC_API(py_##x)

#endif // MACROS_H
