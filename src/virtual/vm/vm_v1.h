/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 2, 2025 at 4:25:28 PM GMT+3
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

#ifndef _VIRTUAL_VM_V1_H_
#define _VIRTUAL_VM_V1_H_

#define USING_STD_API
#include "std/api.h"

#include "virtual/export.h"

typedef struct PRIVATE_API(virtual_vm_methods) {
    const_vm_ptr (*init)(u64 size);
    void (*gc)(const_vm_ptr cvm);
    u64 (*release)(const_vm_ptr cvm, u64 ptr);
    void (*destroy)(const_vm_ptr cvm);
} virtual_vm_methods;

/* definition */
CVM_EXPORT extern const virtual_vm_methods* CALL(vm);

#endif /* _VIRTUAL_VM_V1_H_ */
