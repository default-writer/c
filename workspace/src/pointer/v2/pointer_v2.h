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
#ifndef _POINTER_V2_H_
#define _POINTER_V2_H_

#include "std/common.h"

enum type {
    TYPE_VOID = 0,
    /* value used for pointer type */
    TYPE_PTR = 1,
    /* value used for file type */
    TYPE_FILE = 2,
    /* value used for list type */
    TYPE_LIST = 3
};

struct pointer;

struct pointer_data;

struct list_methods {
    u64 (*alloc)(void);
    void (*free)(u64);
    u64 (*peek)(u64 list_ptr);
    u64 (*pop)(u64 list_ptr);
    void (*push)(u64 list_ptr, u64 ptr);
};

struct file_methods {
    u64 (*file_alloc)(u64 file_path_ptr, u64 mode_ptr);
    u64 (*file_read)(u64 ptr);
    void (*file_free)(u64 ptr);
};

#ifdef USE_MEMORY_DEBUG_INFO
struct debug_methods {
    void (*dump)(struct pointer* ptr);
    void (*dump_ref)(void** ptr);
};
#endif

struct pointer_methods {
    void (*init)(u64 size);
    void (*destroy)(void);
    u64 (*alloc)(void);
    u64 (*copy)(u64 ptr);
    u64 (*peek)(void);
    void (*push)(u64 ptr);
    u64 (*pop)(void);
    void (*strcpy)(u64 dest, u64 src);
    void (*strcat)(u64 dest, u64 src);
    u64 (*match_last)(u64 src_ptr, u64 match_prt);
    u64 (*getcwd)(void);
    u64 (*load)(const char* data);
    void (*printf)(u64 ptr);
    void (*put_char)(u64 ptr, char value);
    char* (*unsafe)(u64 ptr);
    u64 (*size)(u64 ptr);
#ifndef USE_GC
    void (*free)(u64 ptr);
#else
    void (*gc)(void);
#endif
};

void pointer_ctx_init(struct pointer_data** ctx, u64 size);
void pointer_ctx_destroy(struct pointer_data** ctx);

/* definition */
extern const struct pointer_methods pointer_methods_definition;
extern const struct file_methods file_methods_definition;
extern const struct list_methods list_methods_definition;

/* definition */
#if defined(INLINE)
const struct pointer_methods* pointer = &pointer_methods_definition;
const struct file_methods* file = &file_methods_definition;
const struct list_methods* list = &list_methods_definition;
#else
static const struct pointer_methods* pointer = &pointer_methods_definition;
static const struct file_methods* file = &file_methods_definition;
static const struct list_methods* list = &list_methods_definition;
#endif

#endif /* _POINTER_V2_H_ */