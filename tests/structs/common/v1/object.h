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
#ifndef _OBJECT_V1_H_
#define _OBJECT_V1_H_

#include "common/memory.h"
#include "std/common.h"

struct typeinfo;

struct object_methods {
    void* (*create)(struct typeinfo* ti);
    void (*destroy)(struct typeinfo* ti);
};

struct typeinfo {
    void* ptr;
    const size_t size;
#ifdef USE_MEMORY_DEBUG_INFO
    const char* name;
#endif
};

/* definition */
extern const struct object_methods object_methods_definition;

/* definition */
#if defined(INLINE)
const struct object_methods* object = &object_methods_definition;
#else
static const struct object_methods* object = &object_methods_definition;
#endif

#endif /* _OBJECT_V1_H_ */
