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
#ifndef _CLASS_H_
#define _CLASS_H_

#include "std/common.h"

struct object;
struct class;
struct typeinfo;
struct object_typeinfo;

typedef struct object struct_object;
typedef struct typeinfo struct_typeinfo;
typedef struct object_typeinfo struct_object_typeinfo;
typedef struct class struct_class;

typedef struct_object* object;
typedef struct_typeinfo* typeinfo;
typedef struct_object_typeinfo* object_typeinfo;
typedef struct_class* class;

struct object {
    void* ptr;
};

struct typeinfo {
    const size_t size;
#ifdef USE_MEMORY_DEBUG_INFO
    const char* name;
#endif
};

struct class {
    object_typeinfo (*create)(typeinfo ti);
    void (*destroy)(object_typeinfo ptr);
};

struct object_typeinfo {
    const object object;
    const typeinfo typeinfo;
};

#endif /* _CLASS_H_ */