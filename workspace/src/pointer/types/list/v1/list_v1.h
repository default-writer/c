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
#ifndef _POINTER_TYPES_LIST_H_
#define _POINTER_TYPES_LIST_H_

#include "std/common.h"

/*! @file list_v1.h
 *  @brief C API / list
 *
 *  structure for organizing @ref list_methods functions
 *
 */
struct list_methods {
    u64 (*alloc)(void);
    void (*free)(u64 ptr);
    u64 (*peek)(u64 list_ptr);
    /*! returns a list of picked items
        @param list_ptr a list to get items from
        @param nelements number of elements to peek from the list
        @return returns new list which contains n elements from the list in reverse order
    */
    u64 (*peekn)(u64 list_ptr, u64 nelements);
    u64 (*pop)(u64 list_ptr);
    u64 (*popn)(u64 list_ptr, u64 nelements);
    void (*push)(u64 list_ptr, u64 ptr);
    u64 (*size)(u64 ptr);
    void (*release)(u64 ptr);
};

/* definition */
extern const struct list_methods list_methods_definition;

/* definition */
#if defined(INLINE)
const struct list_methods* list = &list_methods_definition;
#else
static const struct list_methods* list = &list_methods_definition;
#endif

#endif /* _POINTER_TYPES_LIST_H_ */