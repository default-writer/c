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
#ifndef _POINTER_V1_H_
#define _POINTER_V1_H_

#include "std/common.h"

/*! @file pointer_v1.h
 *  @brief C API / pointer
 */

enum type {
    /* value used for ephemeral type - null */
    TYPE_NULL = 0,
    /* value used for pointer type - ref */
    TYPE_DATA = 1,
    /* value used for string type - string */
    TYPE_STRING = 2,
    /* value used for string ref type - string ref */
    TYPE_STRING_POINTER = 3,
    /* value used for file type - file */
    TYPE_FILE = 4,
    /* value used for list type - list */
    TYPE_LIST = 5,
    /* value used for object type - object */
    TYPE_OBJECT = 6,
    /* value used for user type - user (id: +0, +1, +2, +3, ...) */
    TYPE_USER = 7
};

struct vm_data;
struct list_data;

/* private */
struct pointer {
    struct vm_data* vm;
    void* data;
    u64 size;
    u64 address;
    u64 id;
};

struct pointer_data {
    struct vm* vm;
#ifdef USE_GC
    struct list_data* gc;
#endif
};

struct pointer_vm_methods {
    struct pointer* (*alloc)(u64 size, u64 id);
    void (*realloc)(struct pointer* ptr, u64 size);
    void (*free)(struct pointer* ptr);
    void (*release)(struct list_data** current);
};

struct pointer_methods {
    void (*init)(u64 size);
    void (*destroy)(void);
    void (*release)(void);
#ifdef USE_VM_DEBUG_INFO
    void (*dump)(struct pointer* ptr);
    void (*dump_ref)(void** ptr);
#endif
#ifdef USE_GC
    void (*gc)(void);
#endif
};

/* definition */
extern const struct pointer_methods pointer_methods_definition;

/* definition */
#if defined(INLINE)
const struct pointer_methods* pointer = &pointer_methods_definition;
#else
static const struct pointer_methods* pointer = &pointer_methods_definition;
#endif

#endif /* _POINTER_V1_H_ */
