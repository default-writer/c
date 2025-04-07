/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 7, 2025 at 3:40:23 AM GMT+3
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

#define USING_VIRTUAL_API

#ifndef _VIRTUAL_API_
#define _VIRTUAL_API_

#define USING_STD_API
#include "std/api.h"

#include "virtual/env/env_v1.h"
#include "virtual/export.h"
#include "virtual/pointer/pointer_v1.h"
#include "virtual/types/data/data_v1.h"
#include "virtual/types/file/file_v1.h"
#include "virtual/types/object/object_v1.h"
#include "virtual/types/stack/stack_v1.h"
#include "virtual/types/string/string_v1.h"
#include "virtual/types/string_pointer/string_pointer_v1.h"
#include "virtual/types/user/user_v1.h"
#include "virtual/virtual/virtual_v1.h"
#include "virtual/vm/vm_v1.h"

/* definition */
CVM_EXPORT extern const virtual_vm_methods* PUBLIC_API(vm);
CVM_EXPORT extern const virtual_methods* PUBLIC_API(virtual);
CVM_EXPORT extern const virtual_pointer_methods* PUBLIC_API(pointer);
CVM_EXPORT extern const virtual_env_methods* PUBLIC_API(env);
CVM_EXPORT extern const virtual_data_methods* PUBLIC_API(data);
CVM_EXPORT extern const virtual_file_methods* PUBLIC_API(file);
CVM_EXPORT extern const virtual_object_methods* PUBLIC_API(object);
CVM_EXPORT extern const virtual_stack_methods* PUBLIC_API(stack);
CVM_EXPORT extern const virtual_string_methods* PUBLIC_API(string);
CVM_EXPORT extern const virtual_string_pointer_methods* PUBLIC_API(string_pointer);
CVM_EXPORT extern const virtual_user_methods* PUBLIC_API(user);

#endif /* _VIRTUAL_API_ */
