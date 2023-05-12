#include "playground/hashtable/v2/hashtable_v2.h"
#include "common/alloc.h"
#include "common/lcg.h"
#include "playground/list/v2/list_v2.h"
#include "playground/pointer/pointer.h"

static u64 lcg_state = 0;

static void hashtable_free_internal(struct hashtable_data* ptr);
static void hashtable_item_free_internal(struct hashtable_data* ptr);

static struct hashtable_data** hashtable; /* pointer table */

static struct hashtable_data* hashtable_alloc(const char* key, void* value);
static void hashtable_free(struct hashtable_data* ptr);
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

static u64 hashtable_size = HASHTABLE_DEFAULT_SIZE;

static u32 (*hash_function_ptr)(const char* source) = 0;

static void hashtable_setup(u32 (*function)(const char* source)) {
    if (function != 0) {
        hash_function_ptr = function;
    }
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
        case 2:
            hash ^= (u32)(*(buf + 1) << 8);
        case 1:
            hash ^= (u32)(*buf);
            hash *= m;
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
    hashtable = _list_alloc(hashtable_size * sizeof(void*));
}

static void hashtable_destroy(void) {
    for (u64 i = 0; i < hashtable_size; i++) {
        hashtable_free(hashtable[i]);
    }
    _list_free(hashtable, hashtable_size * sizeof(void*));
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
    struct hashtable_data* next = hashtable[hash];
    struct hashtable_data* node = _list_alloc(sizeof(struct hashtable_data));
    char* node_key = _list_alloc(strlen(key) + 1); /* +1 for ’\0’ */
    /* char* node_value = _list_alloc(strlen(value) + 1); +1 for ’\0’ */
    strcpy(node_key, key); /* NOLINT */
    node->key = node_key;
    node->value = value;
    node->next = next;
    hashtable[hash] = node;
    return node;
}

static void hashtable_item_free_internal(struct hashtable_data* ptr) {
    _list_free(ptr->key, 0);
    _list_free(ptr, sizeof(struct hashtable_data));
}

static void hashtable_free(struct hashtable_data* ptr) {
    if (ptr != 0) {
        hashtable_free_internal(ptr);
    }
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
        printf("  .$: 0x%016llx !  %16s : 0x%016llx\n", (u64)node, node->key, (u64)node->value);
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
        printf("  >$: 0x%016llx !  %16s : 0x%016llx\n", (u64)node, node->key, (u64)node->value);
    }
#endif
}

/* public */

const struct hashtable hashtable_definition = {
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
    .setup = hashtable_setup
};
