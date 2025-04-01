/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 31, 2025 at 10:33:01 PM GMT+3
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

#ifndef _VIRTUAL_ENV_V1_H_
#define _VIRTUAL_ENV_V1_H_

#define USING_STD_API
#include "std/api.h"

#include "virtual/export.h"

/*! @file env_v1.h
 *  @brief C API / env
 */

typedef struct PRIVATE_API(virtual_env_methods) {
    u64 (*getenv)(const_vm_ptr cvm, u64 name);
    u64 (*getcwd)(const_vm_ptr cvm);
    u64 (*puts)(const_vm_ptr cvm, u64 ptr);
} virtual_env_methods;

/* definition */
CVM_EXPORT extern const virtual_env_methods* CALL(env);

#endif /* _VIRTUAL_ENV_V1_H_ */
