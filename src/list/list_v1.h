/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   February 3, 2025 at 10:04:39 PM GMT+3
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

#ifndef _LIST_MICRO_DATA_H_
#define _LIST_MICRO_DATA_H_

#include "std/api.h"

typedef struct PRIVATE_API(list) {
    /* initialize context */
    void (*init)(stack_pointer* current);
    /* destroy context */
    void (*destroy)(stack_pointer* current);
    /* push item on current context (stack) */
    void (*push)(stack_pointer* current, void* item);
    /* pop item on current context (stack) */
    void* (*pop)(stack_pointer* current);
    /* peek item on current context (stack) */
    void* (*peek)(stack_pointer* current);
#ifdef USE_MEMORY_DEBUG_INFO
    /* print head */
    void (*print_head)(stack_pointer* current);
    /* print */
    void (*print)(stack_pointer* current);
#endif
} list;

/* definition */
extern const list PRIVATE_API(list_definition);

/* definition */
#ifdef INLINE
const list_v1* list_v1 = &PRIVATE_API(list_definition);
#else
/* definition */
static const list* list_v1 = &PRIVATE_API(list_definition);
#endif

#endif /* _LIST_MICRO_DATA_H_ */
