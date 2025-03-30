/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 22, 2025 at 11:23:30 AM GMT+3
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

#define USING_SYSTEM_INFO

#ifndef _SYSTEM_INFO_H_
#define _SYSTEM_INFO_H_

#define USING_STD_API
#include "std/api.h"

#include "system/export.h"

typedef struct PRIVATE_API(system_info_methods) {
    const u64 timestamp;
    const char* commit;
    const char* version;
    u64 POINTER_TYPE_SIZE;
} system_info_methods;

/* definition */
CSYS_EXPORT extern const system_info_methods* CALL(info);

#endif /* _SYSTEM_INFO_H_ */
