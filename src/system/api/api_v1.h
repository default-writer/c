/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 7, 2025 at 3:39:59 AM GMT+3
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

#define USING_SYSTEM_API

#ifndef _SYSTEM_API_
#define _SYSTEM_API_

#define USING_STD_API
#include "std/api.h"

#include "system/export.h"

#include "system/error/error_v1.h"
#include "system/hashtable/hashtable_v1.h"
#include "system/info/info_v1.h"
#include "system/list/list_v1.h"
#include "system/memory/memory_v1.h"
#include "system/options/options_v1.h"
#include "system/os/os_v1.h"

/* definition */
CSYS_EXPORT extern const system_error_methods* PUBLIC_API(error);
CSYS_EXPORT extern const system_info_methods* PUBLIC_API(info);
CSYS_EXPORT extern const system_list_methods* PUBLIC_API(list);
CSYS_EXPORT extern const system_memory_methods* PUBLIC_API(memory);
CSYS_EXPORT extern const system_options_methods* PUBLIC_API(options);
CSYS_EXPORT extern const system_os_methods* PUBLIC_API(os);
CSYS_EXPORT extern const system_hashtable_methods* PUBLIC_API(hashtable);

#endif /* _SYSTEM_API_ */
