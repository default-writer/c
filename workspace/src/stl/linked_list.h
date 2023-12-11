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

#ifndef _STL_LINKED_LIST_H_
#define _STL_LINKED_LIST_H_

#include "std/common.h"

struct linked_list;
struct linked_list_node;

typedef u64 (*linked_list_node_match_function)(struct linked_list_node* current);

struct linked_list_methods {
    struct linked_list* (*new)(void);
    void (*delete)(struct linked_list** list);
    void (*reverse)(struct linked_list* list);
    void (*reverse_until)(struct linked_list* list, struct linked_list_node* node);
    void (*reverse_until_match)(struct linked_list* list, linked_list_node_match_function match);
    struct linked_list_node* (*append_head)(struct linked_list* list, void* data);
    struct linked_list_node* (*append_tail)(struct linked_list* list, void* data);
    u64 (*count)(struct linked_list* list);
    void* (*data)(struct linked_list_node* list);
    struct linked_list_node* (*next)(struct linked_list_node* node);
    struct linked_list_node* (*head)(struct linked_list* list);
    struct linked_list_node* (*tail)(struct linked_list* list);
    void (*set_next)(struct linked_list_node* node, struct linked_list_node* value);
    void (*set)(struct linked_list* list, struct linked_list_node* head, struct linked_list_node* tail);
};

#endif /* _STL_LINKED_LIST_H_ */
