/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 27, 2025 at 10:25:46 AM GMT+3
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
    ID_ERROR_INVALID_POINTER,
    ID_ERROR_INVALID_ARGUMENT,
    ID_ERROR_INVALID_TYPE_ID,
    ID_ERROR_INVALID_VALUE
};

typedef struct PRIVATE_API(system_error_methods) {
#ifdef USE_MEMORY_DEBUG_INFO
    void (*stderr)(enum error_message_code id, const char* func, const char* file, int line, const char* format, ...);
#else
    void (*stderr)(enum error_message_code id, const char* format, ...);
#endif
} system_error_methods;

/* public */
CSYS_EXPORT extern const system_error_methods* CALL(error);

#endif /* _SYSTEM_ERROR_H_ */
