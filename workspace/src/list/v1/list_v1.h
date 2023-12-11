/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:15:15 GMT+3
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

#ifndef _LIST_DATA_V1_H_
#define _LIST_DATA_V1_H_

#include "std/common.h"

struct list_v1 {
    /* points to previous node */
    struct list_v1* prev;
    /* points to next node */
    struct list_v1* next;
    /* data */
    void* data;
};

struct list_methods_v1 {
    /* allocate list item */
    struct list_v1* (*alloc)(void* payload);
    /* free item */
    void (*free)(struct list_v1** item);
    /* initialize context */
    void (*init)(struct list_v1** current);
    /* destroy context */
    void (*destroy)(struct list_v1** current);
    /* push item on current context (stack) */
    struct list_v1* (*push)(struct list_v1** current, struct list_v1* item);
    /* pop item on current context (stack) */
    struct list_v1* (*pop)(struct list_v1** current);
    /* peek item on current context (stack) */
    struct list_v1* (*peek)(struct list_v1** current);
#ifdef USE_VM_DEBUG_INFO
    /* print head */
    void (*print_head)(struct list_v1** current);
    /* print */
    void (*print)(struct list_v1** current);
#endif
};

#endif /* _LIST_DATA_V1_H_ */
