/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   March 28, 2025 at 11:35:14 AM GMT+3
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

#include "hashtable_v1.h"

#include "system/api/api_v1.h"

#define HASHTABLE_TYPE_SIZE sizeof(hashtable_type)
#define HASHENTRY_TYPE_SIZE sizeof(hashentry_type)
#define HASHENTRY_PTR_SIZE sizeof(hashentry_ptr)
#define HASHENTRY_PTR_ARRAY_SIZE(size) (size * HASHENTRY_PTR_SIZE)

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
    ht->table = (hashentry_ptr*)api->realloc(ht->table, HASHENTRY_PTR_ARRAY_SIZE(ht->capacity * 2));
    api->memset(ht->table + ht->size, 0x00, 8 * (u64)((double)ht->capacity * (2 - HASHTABLE_LOAD_FACTOR)));
    ht->capacity = ht->capacity * 2;
    return TRUE;
}

/* implementation */
hashtable_ptr hashtable_init() {
    hashtable_ptr ht = (hashtable_ptr)api->alloc(1, HASHTABLE_TYPE_SIZE);
    ht->capacity = HASHTABLE_INITIAL_CAPACITY;
    ht->size = 0;
    ht->table = (hashentry_ptr*)api->alloc(ht->capacity, HASHENTRY_PTR_SIZE);
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
    hashentry_ptr entry = api->alloc(1, HASHENTRY_TYPE_SIZE);
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
        return NULL;
    }
    u64 index = hashtable_function_internal(key);
    hashentry_ptr entry = ht->table[index];
    while (entry != NULL) {
        if (entry->key == key) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
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
            api->free(entry);
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
            api->free(entry);
            entry = next;
        }
    }
    api->free(ht->table);
    api->free(ht);
}

/* public */
const virtual_hashtable_methods PRIVATE_API(virtual_hashtable_methods_definitions) = {
    .init = hashtable_init,
    .insert = hashtable_insert,
    .get = hashtable_get,
    .remove = hashtable_remove,
    .destroy = hashtable_destroy
};

const virtual_hashtable_methods* hashtable = &PRIVATE_API(virtual_hashtable_methods_definitions);
const virtual_hashtable_methods* CALL(hashtable) {
    return hashtable;
}
