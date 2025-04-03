/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 2, 2025 at 10:15:48 PM GMT+3
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
