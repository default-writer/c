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
