/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
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