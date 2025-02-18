/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 18, 2025 at 12:24:45 AM GMT+3
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
#ifndef _SYS_INFO_H_
#define _SYS_INFO_H_

#include "std/api.h"

#include "sys/export.h"

typedef struct PRIVATE_API(info_methods) {
    const u64 timestamp;
    const char* commit;
    const char* version;
} info_methods;

#if !defined(_WIN32)
/* definition */
CSYS_EXPORT extern const info_methods PRIVATE_API(info_methods_definitions);

/* definition */
#ifdef INLINE
const info_methods* info = &PRIVATE_API(info_methods_definitions);
#else
/* definition */
CSYS_EXPORT extern const info_methods* sys_info;
#endif

#else

CSYS_EXPORT extern const info_methods* _sys_info();

#define sys_info _sys_info()

#endif

#endif /* _SYS_INFO_H_ */
