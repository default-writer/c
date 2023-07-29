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
#ifndef _STL_LINKED_LIST_ENUMERATOR_H_
#define _STL_LINKED_LIST_ENUMERATOR_H_

#include "std/common.h"

struct linked_list;
struct linked_list_node;
struct linked_list_enumerator;

struct linked_list_enumerator_methods {
    struct linked_list_enumerator* (*new)(struct linked_list* list);
    void (*delete)(struct linked_list_enumerator** enumerator);
    void (*reverse)(struct linked_list_enumerator* enumerator);
    struct linked_list_node* (*next)(struct linked_list_enumerator* enumerator);
    struct linked_list_node* (*find)(struct linked_list_enumerator* enumerator, void* data);
    struct linked_list_node* (*find_prev)(struct linked_list_enumerator* enumerator, void* data);
};

#endif /* _STL_LINKED_LIST_ENUMERATOR_H_ */
