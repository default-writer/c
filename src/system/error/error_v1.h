/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 14, 2025 at 11:26:15 AM GMT+3
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
    ID_ERROR_VM_NOT_INITIALIZED,
    ID_ERROR_POINTER_NOT_INITIALIZED,
    ID_ERROR_ADDRESS_NOT_INITIALIZED,
    ID_ERROR_ARGUMENT_VALUE_NOT_INITIALIZED,
    ID_ERROR_INVALID_CONDITION
};

typedef struct error_api {
#ifdef USE_MEMORY_DEBUG_INFO
    void (*stderr)(enum error_message_code id, const char* func, const char* args, const char* file, int line);
#else
    void (*stderr)(enum error_message_code id);
#endif
} error_api_type;

/* api */
CSYS_EXPORT extern const error_api_type* error_api;

#endif /* _SYSTEM_ERROR_H_ */
