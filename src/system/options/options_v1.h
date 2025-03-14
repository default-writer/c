/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 14, 2025 at 7:28:13 AM GMT+3
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

#define USING_SYSTEM_OPTIONS

#ifndef _SYSTEM_OPTIONS_H_
#define _SYSTEM_OPTIONS_H_

#define USING_STD_API
#include "std/api.h"

#include "sys/export.h"

typedef struct PRIVATE_API(options_methods) {
    const u64 size;
} options_methods;

/* definition */
CSYS_EXPORT extern const options_methods* CALL(system_options);

#endif /* _SYSTEM_OPTIONS_H_ */
