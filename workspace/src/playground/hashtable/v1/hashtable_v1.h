/*
 *
 * Russian's IP Protection License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person with citizenship
 * and location in Russia including Crimea and all occupations obtaining a copy
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
 * For the rest of the world it is an order to pay royalties by agreement to the
 * author of the code base for ability to use any part of the project for any
 * purpouse including but not limited to the creative ideas or technologies are
 * being used in this owned intellectual property.
 *
 * It is strictly prohibited to use this code base or any part of it for any purpouse
 * including prohibiting or restricive purpouses agains Russians for any EU citizens
 * or other person with USA citizenship, origin or background including work permit
 * or locations from selected territories or any territory or any other country except
 * Russia considered as breaking basic human rights, freedom of speesh or involved in
 * acts of terrorism in a territory owned, occupied or managed by another country.
 *
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
