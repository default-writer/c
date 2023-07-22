/*
 *
 *  MIT License
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
#ifndef _LIST_DATA_H_
#define _LIST_DATA_H_

#include "std/common.h"

struct list_data {
    /* points to previous node */
    struct list_data* prev;
    /* points to next node */
    struct list_data* next;
    /* data */
    void* data;
};

struct list {
    /* allocate list item */
    struct list_data* (*alloc)(void* payload);
    /* free item */
    void (*free)(struct list_data** item);
    /* initialize context */
    void (*init)(struct list_data** current);
    /* destroy context */
    void (*destroy)(struct list_data** current);
    /* push item on current context (stack) */
    struct list_data* (*push)(struct list_data** current, struct list_data* item);
    /* pop item on current context (stack) */
    struct list_data* (*pop)(struct list_data** current);
    /* peek item on current context (stack) */
    struct list_data* (*peek)(struct list_data** current);
#ifdef VM_DEBUG_INFO
    /* print head */
    void (*print_head)(struct list_data** current);
    /* print */
    void (*print)(struct list_data** current);
#endif
};

#endif /* _LIST_DATA_H_ */
