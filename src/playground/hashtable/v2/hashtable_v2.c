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
#include "playground/hashtable/v2/hashtable_v2.h"
#include "common/alloc.h"
#include "common/lcg.h"
#include "playground/list/v2/list_v2.h"
#include "pointer/v2/pointer_v2.h"

/* macros */
#define DEFAULT_SIZE 101
#define PTR_SIZE sizeof(void*) /* size of a pointer */
#define ALLOC_SIZE(size) (size * PTR_SIZE)

struct hashtable_data { /* table entry: */
    struct hashtable_data* next; /* next entry in chain */
    char* key; /* defined name */
    u64 key_size; /* key size */
    /* char* value; replacement text */
    void* value;
    u64 value_size; /* value size */
};

/* public */
u32 default_hash(const char* source);

/* definition */
extern const struct memory memory_definition;
static const struct memory* memory = &memory_definition;

/* private */

static u64 lcg_state = 0;

static void hashtable_free_internal(struct hashtable_data* ptr);
static void hashtable_item_free_internal(struct hashtable_data* ptr);

static struct hashtable_data** hashtable; /* pointer table */

static struct hashtable_data* hashtable_alloc(const char* key, void* value);
static void hashtable_free(struct hashtable_data* ptr);
static const char* hashtable_key(const struct hashtable_data* ptr);
static const void* hashtable_data(const struct hashtable_data* ptr);
static struct hashtable_data* hashtable_find(const char* key);
static struct hashtable_data* hashtable_value(u32 index);
static u32 hashtable_get(const char* key);
static u32 hashtable_hash(const char* key);
static void hashtable_set(const char* key, void* value);
static void hashtable_setup(u32 (*function)(const char* source));
static u32 default_hash_function(const char* source);
static u32 hash_func(const char* source);

static void hashtable_init(u64 size);
static void hashtable_destroy(void);

/* implementation */

static u64 hashtable_size = HASHTABLE_DEFAULT_SIZE;

static u32 (*hash_function_ptr)(const char* source) = 0;

static void hashtable_setup(u32 (*function)(const char* source)) {
    hash_function_ptr = function;
}

/* hash: form hash value for string s */
static u32 hash_func(const char* source) {
    if (hash_function_ptr != 0) {
        return hash_function_ptr(source);
    }
    return default_hash_function(source);
}

/* hash: form hash value for string s */
static u32 default_hash_function(const char* source) {
    u32 data = 0;
    if (source != 0) {
        data = default_hash(source);
    }
    return data;
}

/* hash: form hash value for string s */
u32 default_hash(const char* source) {
    /* One-byte-at-a-time hash based on Murmur's mix */
    /* Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    u32 data = 0;
    if (source != 0) {
        u32 hash = (u32)~0x5a32b847;
        const char* ptr = source;
        while (*ptr != 0) {
            hash ^= (u32)*ptr;
            hash *= 0x5bd1e995;
            hash ^= hash >> 15;
            ptr++;
        }
        data = hash;
    }
    return data;
}

/* hash: form hash value for string s */
u32 murmurhash3(const char* source) {
    u32 data = 0;
    if (source != 0) {
        u32 len = 0;
        const u8* ptr = (const u8*)source;
        while (*ptr != 0) {
            ptr++;
        }
        len = (u32)(ptr - (const u8*)source);

        const u32 m = 0x5bd1e995;
        u32 hash = lcg_state ^ len;

        const u8* buf = (const u8*)source;

        /* Mix 4 bytes at a time into the hash. */
        while (len >= 4) {
            u32 p0 = (u32)(*(buf + 0));
            u32 p1 = (u32)(*(buf + 1) << 8);
            u32 p2 = (u32)(*(buf + 2) << 16);
            u32 p3 = (u32)(*(buf + 3) << 24);
            u32 k = p0 | p1 | p2 | p3;
            k *= m;
            k ^= k >> 24;
            k *= m;
            hash *= m;
            hash ^= k;
            buf += 4;
            len -= 4;
        }

        /* Handle the last few bytes of the input array. */
        switch (len) {
        case 3:
            hash ^= (u32)(*(buf + 2) << 16); /* NOLINT */
#if !defined(__GNUC__)
            FALL_THROUGH;
#endif
        /* fall through */ case 2:
            hash ^= (u32)(*(buf + 1) << 8);
#if !defined(__GNUC__)
            FALL_THROUGH;
#endif
            /* fall through */
        case 1:
            hash ^= (u32)(*buf);
            hash *= m;
#if !defined(__GNUC__)
            FALL_THROUGH;
#endif
            /* fall through */
        default:
            break;
        }

        /* Do a few final mixes of the hash. */
        hash ^= hash >> 13;
        hash *= m;
        hash ^= hash >> 15;
        data = hash;
    }
    return data;
}

static void hashtable_init(u64 size) {
    if (size > 0) {
        hashtable_size = size;
    }
    hashtable = memory->alloc(ALLOC_SIZE(hashtable_size));
}

static void hashtable_destroy(void) {
    for (u64 i = 0; i < hashtable_size; i++) {
        hashtable_free(hashtable[i]);
    }
    memory->free(hashtable, ALLOC_SIZE(hashtable_size));
}

static void hashtable_free_internal(struct hashtable_data* ptr) {
    struct hashtable_data* node;
    do {
        u32 hash = hashtable_get(ptr->key);
        node = hashtable[hash];
        struct hashtable_data* current = 0;
        while (node != 0 && node != ptr) {
            current = node;
            node = node->next;
        }
        if (node != 0) {
            if (current != 0) {
                current->next = node->next;
            }
            if (hashtable[hash] != node) {
                hashtable_item_free_internal(node);
                break;
            }
            hashtable[hash] = 0;
        }
        node = ptr->next;
        hashtable_item_free_internal(ptr);
        ptr = node;
    } while (node != 0);
}

static struct hashtable_data* hashtable_alloc(const char* key, void* value) {
    u32 hash = hash_func(key) % hashtable_size;
    if (key == 0) {
        return hashtable[hash];
    }
    struct hashtable_data* next = hashtable[hash];
    struct hashtable_data* node = memory->alloc(sizeof(struct hashtable_data));
    u64 size = strlen(key) + 1;
#ifdef USE_MEMORY_DEBUG_INFO
    node->key_size = size;
#endif
    char* node_key = memory->alloc(size); /* +1 for ’\0’ */
    strcpy(node_key, key); /* NOLINT */
    node->key = node_key;
    node->value = value;
    node->next = next;
    hashtable[hash] = node;
    return node;
}

static void hashtable_item_free_internal(struct hashtable_data* ptr) {
    u64 size = 0;
#ifdef USE_MEMORY_DEBUG_INFO
    size = ptr->key_size;
#endif
    memory->free(ptr->key, size);
    memory->free(ptr, sizeof(struct hashtable_data));
}

static void hashtable_free(struct hashtable_data* ptr) {
    if (ptr != 0) {
        hashtable_free_internal(ptr);
    }
}

static const char* hashtable_key(const struct hashtable_data* ptr) {
    const char* data = ptr != 0 ? ptr->key : 0;
    return data;
}

static const void* hashtable_data(const struct hashtable_data* ptr) {
    const void* data = ptr != 0 ? ptr->value : 0;
    return data;
}

static struct hashtable_data* hashtable_find(const char* key) {
    u32 hash = hashtable_get(key);
    struct hashtable_data* node = hashtable[hash];
    while (node != 0) {
        if (node->next == 0) {
            break;
        }
        node = node->next;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    if (node != 0) {
        printf("  .$: %016llx ! %16s : %016llx\n", (u64)node, node->key, (u64)node->value);
    }
#endif
    return node;
}

static struct hashtable_data* hashtable_value(u32 hash) {
    return hashtable[hash % hashtable_size];
}

static u32 hashtable_get(const char* key) {
    u32 hash = hash_func(key) % hashtable_size;
    return hash;
}

static u32 hashtable_hash(const char* key) {
    u32 hash = hash_func(key) % hashtable_size;
    return hash;
}

static u32 hashtable_count(const char* key) {
    u32 count = 0;
    u32 hash = hashtable_get(key);
    if (hashtable != 0) {
        struct hashtable_data* node = hashtable[hash];
        while (node != 0) {
            node = node->next;
            count++;
        }
    }
    return count;
}

static void hashtable_set(const char* key, void* value) {
    u32 hash = hash_func(key) % hashtable_size;
    struct hashtable_data* node = hashtable[hash];
    if (node != 0) {
        node->value = value;
    } else {
        node = hashtable_alloc(key, value);
    }
#ifdef USE_MEMORY_DEBUG_INFO
    if (node != 0) {
        printf("  >$: %016llx ! %16s : %016llx\n", (u64)node, node->key, (u64)node->value);
    }
#endif
}

/* public */

const struct hashtable hashtable_definition_v2 = {
    .init = hashtable_init,
    .destroy = hashtable_destroy,
    .alloc = hashtable_alloc,
    .free = hashtable_free,
    .find = hashtable_find,
    .get = hashtable_get,
    .hash = hashtable_hash,
    .count = hashtable_count,
    .value = hashtable_value,
    .set = hashtable_set,
    .setup = hashtable_setup,
    .key = hashtable_key,
    .data = hashtable_data
};
