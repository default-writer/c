/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 18, 2025 at 12:20:44 AM GMT+3
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

#ifndef _VM_V1_OPTIONS_H_
#define _VM_V1_OPTIONS_H_

#include "std/api.h"

#include "sys/export.h"

typedef struct PRIVATE_API(options_methods) {
    const u64 size;
} options_methods;

#if !defined(_WIN32)
/* definition */
CSYS_EXPORT extern const options_methods PRIVATE_API(options_methods_definitions);

/* definition */
#ifdef INLINE
const options_methods* sys_options = &PRIVATE_API(options_methods_definitions);
#else
/* definition */
CSYS_EXPORT extern const options_methods* sys_options;
#endif

#else

CSYS_EXPORT extern const options_methods* _sys_options();

#define sys_options _sys_options()

#endif

#endif /* _VM_V1_OPTIONS_H_ */
