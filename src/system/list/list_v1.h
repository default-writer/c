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

#define USING_SYSTEM_LIST

#ifndef _SYSTEM_LIST_H_
#define _SYSTEM_LIST_H_

#define USING_STD_API
#include "std/api.h"

#include "system/export.h"

typedef struct PRIVATE_API(system_list_methods) {
    /* initialize context */
    void (*init)(stack_ptr* current);
    /* push item on current context (stack) */
    void (*push)(stack_ptr* current, void* item);
    /* pop item on current context (stack) */
    void* (*pop)(stack_ptr* current);
    /* peek item on current context (stack) */
    void* (*peek)(stack_ptr* current);
    /* destroy context */
    void (*destroy)(stack_ptr* current);
#ifdef USE_MEMORY_DEBUG_INFO
    /* print head */
    void (*print_head)(stack_ptr* current);
    /* print */
    void (*print)(stack_ptr* current);
#endif
} system_list_methods;

/* definition */
CSYS_EXPORT extern const system_list_methods* CALL(list);

#endif /* _SYSTEM_LIST_H_ */
