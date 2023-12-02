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

struct pointer;
struct vm_data;
struct vm_type;

struct pointer_methods {
    void (*init)(u64 size);
    void (*destroy)(void);
    void (*gc)(void);
    struct pointer* (*alloc)(u64 size, u64 id);
    void (*realloc)(struct pointer* ptr, u64 size);
    u64 (*register_type)(u64 id, const struct vm_type* type);
    void (*free)(u64 ptr);
    u64 (*address)(const struct pointer* ptr);
    struct vm_data* (*vm)(const struct pointer* ptr);
    void (*release)(struct pointer* ptr);
    u64 (*size)(const struct pointer* ptr);
    void* (*read)(const struct pointer* ptr);
    u64 (*read_type)(const struct pointer* ptr, u64 id);
    void (*write)(struct pointer* ptr, struct vm_data* vm, u64 address);
#if defined(VM_MEMORY_DEBUG_INFO)
    void (*dump)(struct pointer* ptr);
    void (*dump_ref)(void** ptr);
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
