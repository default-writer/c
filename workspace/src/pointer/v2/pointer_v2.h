/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:15:15 GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
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

#if defined(VM_MEMORY_DEBUG_INFO)
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
