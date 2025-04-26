/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 21, 2025 at 9:25:59 AM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "hashtable_v1.h"

#include "system/os/os_v1.h"

#define HASHTABLE_TYPE_SIZE sizeof(hashtable_type)
#define HASHENTRY_TYPE_SIZE sizeof(hashentry_type)

/* definition */
static hashtable_ptr hashtable_init(void);
static void hashtable_destroy(hashtable_ptr ht);
static hashentry_ptr hashtable_insert(hashtable_ptr ht, u64 key, const_void_ptr value);
static const_void_ptr hashtable_get(hashtable_ptr ht, u64 key);
static u64 hashtable_remove(hashtable_ptr ht, u64 key);

/* internal */
static u64 hashtable_function_internal(u64 key);
static u64 hashtable_resize_internal(hashtable_ptr ht);

/* implementation */
u64 hashtable_function_internal(u64 key) {
    return key % HASHTABLE_INITIAL_CAPACITY;
}

u64 hashtable_resize_internal(hashtable_ptr ht) {
    ht->table = (hashentry_ptr*)CALL(os)->realloc(ht->table, PTR_ARRAY_SIZE(ht->capacity * 2));
    CALL(os)->memset(ht->table + ht->size, 0x00, 8 * (u64)((double)ht->capacity * (2 - HASHTABLE_LOAD_FACTOR)));
    ht->capacity = ht->capacity * 2;
    return TRUE;
}

/* implementation */
hashtable_ptr hashtable_init() {
    hashtable_ptr ht = (hashtable_ptr)CALL(os)->calloc(1, HASHTABLE_TYPE_SIZE);
    ht->capacity = HASHTABLE_INITIAL_CAPACITY;
    ht->size = 0;
    ht->table = (hashentry_ptr*)CALL(os)->calloc(ht->capacity, PTR_SIZE);
    return ht;
}

hashentry_ptr hashtable_insert(hashtable_ptr ht, u64 key, const_void_ptr value) {
    if (ht == NULL) {
        return NULL_PTR;
    }
    if ((long double)ht->size / ht->capacity >= HASHTABLE_LOAD_FACTOR) {
        hashtable_resize_internal(ht);
    }
    u64 index = hashtable_function_internal(key);
    hashentry_ptr entry = CALL(os)->calloc(1, HASHENTRY_TYPE_SIZE);
    *entry = (hashentry_type) {
        .key = key,
        .value = value,
        .next = ht->table[index]
    };
    ht->table[index] = entry;
    ht->size++;
    return entry;
}

const_void_ptr hashtable_get(hashtable_ptr ht, u64 key) {
    if (ht == NULL) {
        return NULL_PTR;
    }
    u64 index = hashtable_function_internal(key);
    hashentry_ptr entry = ht->table[index];
    while (entry != NULL) {
        if (entry->key == key) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL_PTR;
}

u64 hashtable_remove(hashtable_ptr ht, u64 key) {
    if (ht == NULL) {
        return FALSE;
    }
    u64 index = hashtable_function_internal(key);
    hashentry_ptr entry = ht->table[index];
    hashentry_ptr prev = NULL;
    while (entry != NULL) {
        if (entry->key == key) {
            if (prev != NULL) {
                prev->next = entry->next;
            } else {
                ht->table[index] = entry->next;
            }
            CALL(os)->free(entry);
            ht->size--;
            return TRUE;
        }
        prev = entry;
        entry = entry->next;
    }
    return FALSE;
}

void hashtable_destroy(hashtable_ptr ht) {
    if (ht == NULL)
        return;
    for (u64 i = 0; i < ht->capacity; i++) {
        hashentry_ptr entry = ht->table[i];
        while (entry != NULL) {
            hashentry_ptr next = entry->next;
            CALL(os)->free(entry);
            entry = next;
        }
    }
    CALL(os)->free(ht->table);
    CALL(os)->free(ht);
}

/* public */
const system_hashtable_methods PRIVATE_API(system_hashtable_methods_definitions) = {
    .init = hashtable_init,
    .insert = hashtable_insert,
    .get = hashtable_get,
    .remove = hashtable_remove,
    .destroy = hashtable_destroy
};

const system_hashtable_methods* PRIVATE_API(hashtable) = &PRIVATE_API(system_hashtable_methods_definitions);
const system_hashtable_methods* CALL(hashtable) {
    return PRIVATE_API(hashtable);
}
