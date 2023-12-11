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

#ifndef _STL_LINKED_LIST_STATIC_ENUMERATOR_H_
#define _STL_LINKED_LIST_STATIC_ENUMERATOR_H_

#include "std/common.h"

struct linked_list;
struct linked_list_node;

struct linked_list_static_enumerator_methods {
    void (*init)(struct linked_list* list);
    void (*destroy)(void);
    void (*reverse)(void);
    struct linked_list_node* (*next)(void);
    struct linked_list_node* (*find)(void* data);
    struct linked_list_node* (*find_prev)(void* data);
};

#endif /* _STL_LINKED_LIST_ENUMERATOR_H_ */
