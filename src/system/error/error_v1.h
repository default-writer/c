/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 17, 2025 at 3:32:27 AM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#define USING_SYSTEM_ERROR

#ifndef _SYSTEM_ERROR_H_
#define _SYSTEM_ERROR_H_

#define USING_STD_API
#include "std/api.h"

#include "system/export.h"

enum error_message_code {
    ID_ERROR_NO_ERROR,
    ID_ERROR_NO_MEMORY,
    ID_ERROR_NULLPTR,
    ID_ERROR_INVALID_ADDRESS,
    ID_ERROR_VM_NOT_INITIALIZED,
    ID_ERROR_POINTER_NOT_INITIALIZED,
    ID_ERROR_ADDRESS_NOT_INITIALIZED,
    ID_ERROR_ARGUMENT_VALUE_NOT_INITIALIZED,
    ID_ERROR_INVALID_CONDITION
};

typedef struct error_api {
    void (*clear)(void);
    u64 (*get_error_code)(void);
    void (*get_stack_trace)(void);
#ifdef USE_MEMORY_DEBUG_INFO
    void (*set_error_code)(u64 error_code, const char* error_func, const char* error_args, const char* error_file, int error_line);
#else
    void (*set_error_code)(u64 code);
#endif
} error_api_type;

/* api */
CSYS_EXPORT extern const error_api_type* error_api;

#endif /* _SYSTEM_ERROR_H_ */
