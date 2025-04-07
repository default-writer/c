/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   April 7, 2025 at 3:38:49 AM GMT+3
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

#ifndef _SYSTEM_HASHTABLE_V1_H_
#define _SYSTEM_HASHTABLE_V1_H_

#define USING_STD_API
#include "std/api.h"

#include "system/export.h"

#define HASHTABLE_INITIAL_CAPACITY 16
#define HASHTABLE_LOAD_FACTOR 0.75

typedef struct PRIVATE_API(system_hashtable_methods) {
    hashtable_ptr (*init)(void);
    hashentry_ptr (*insert)(hashtable_ptr ht, u64 key, const_void_ptr value);
    const_void_ptr (*get)(hashtable_ptr ht, u64 key);
    u64 (*remove)(hashtable_ptr ht, u64 key);
    void (*destroy)(hashtable_ptr ht);
} system_hashtable_methods;

/* definition */
CSYS_EXPORT extern const system_hashtable_methods* CALL(hashtable);

#endif /* _SYSTEM_HASHTABLE_V1_H_ */
