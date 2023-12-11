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

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#define HASHTABLE_DEFAULT_SIZE 101

#include "std/common.h"

struct hashtable_data { /* table entry: */
    struct hashtable_data* next; /* next entry in chain */
    char* key; /* defined name */
    u64 key_size; /* key_size */
    char* value; /* replacement text */
    u64 value_size; /* value_size*/
};

struct hashtable {
    void (*init)(u32 size);
    void (*destroy)(void);
    struct hashtable_data* (*alloc)(const char* key, const char* value);
    void (*free)(struct hashtable_data* node);
    struct hashtable_data* (*find)(const char* key);
    struct hashtable_data* (*get)(const char* key);
    u32 (*hash)(const char* key);
    u32 (*count)(const char* key);
    void (*set)(const char* key, const char* value);
    void (*setup)(u32 (*function)(const char* source));
};

u32 default_hash(const char* source);
u32 murmurhash3(const char* source);

#endif /* _HASHTABLE_H_ */
