#include "playground/hashtable/hashtable.h"
#include "common/alloc.h"
#include "common/lcg.h"
#include "playground/list/v2/list_v2.h"
#include "playground/pointer/pointer.h"

#define DEFAULT_SIZE 101

static u64 lcg_state = 0;

static struct hashtable_data* hashtable_extract_internal(struct hashtable_data* head, const struct hashtable_data* ptr);

static struct hashtable_data** hashtable; /* pointer table */

static struct hashtable_data* hashtable_alloc(const char* key, const char* value);
static void hashtable_free(struct hashtable_data* node);
static struct hashtable_data* hashtable_find(const char* key);
static struct hashtable_data* hashtable_get(const char* key);
static void hashtable_set(const char* key, const char* value);
static void hashtable_setup(u32 (*function)(const char* source));
static void update(char** prev, const char* new);
static u32 hash_func(const char* source);

static void hashtable_init(u64 size);
static void hashtable_destroy(void);

static u64 hashtable_size = DEFAULT_SIZE;

static u32 (*hash_function_ptr)(const char* source) = murmurhash3;

static void hashtable_setup(u32 (*function)(const char* source)) {
    if (hash_function_ptr != 0) {
        hash_function_ptr = function;
    }
}

/* hash: form hash value for string s */
static u32 hash_func(const char* source) {
    return hash_function_ptr(source);
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
            u32 p0 = (u32)buf[0];
            u32 p1 = (u32)(buf[1] << 8);
            u32 p2 = (u32)(buf[2] << 16);
            u32 p3 = (u32)(buf[3] << 24);
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
            hash ^= (u32)(buf[2] << 16); /* NOLINT */
#if !defined(__GNUC__)
            FALL_THROUGH;
#endif
            /* fall through */
        case 2:
            hash ^= (u32)(buf[1] << 8);
#if !defined(__GNUC__)
            FALL_THROUGH;
#endif
            /* fall through */
        case 1:
            hash ^= (u32)(buf[0]);
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
    hashtable = _list_alloc(hashtable_size * sizeof(void*));
}

static void hashtable_destroy(void) {
    for (u64 i = 0; i < hashtable_size; i++) {
        hashtable_free(hashtable[i]);
    }
    _list_free(hashtable, hashtable_size * sizeof(void*));
}

static struct hashtable_data* hashtable_extract_internal(struct hashtable_data* head, const struct hashtable_data* ptr) {
    struct hashtable_data* node = head;
    struct hashtable_data* prev = 0;
    while (node != 0 && node != ptr) {
        prev = node;
        node = node->next;
    }
    if (node != 0 && prev != 0) {
        prev->next = node->next;
    }
    return node;
}

static struct hashtable_data* hashtable_alloc(const char* key, const char* value) {
    struct hashtable_data* node = _list_alloc(sizeof(struct hashtable_data));
    update(&node->key, key);
    update(&node->value, value);
    u32 hash = hash_func(key) % hashtable_size;
    struct hashtable_data* next = hashtable[hash];
    node->next = next;
    hashtable[hash] = node;
    return node;
}

static void hashtable_free(struct hashtable_data* node) {
    if (node != 0) {
        struct hashtable_data* ptr = node;
        struct hashtable_data* next;
        do {
            u32 hash = hash_func(ptr->key) % hashtable_size;
            const struct hashtable_data* found = hashtable_extract_internal(hashtable[hash], ptr);
            if (hashtable[hash] != found) {
                _list_free(ptr->key, 0);
                _list_free(ptr->value, 0);
                _list_free(ptr, sizeof(struct hashtable_data));
                break;
            } else {
                hashtable[hash] = 0;
            }
            next = ptr->next;
            _list_free(ptr->key, 0);
            _list_free(ptr->value, 0);
            _list_free(ptr, sizeof(struct hashtable_data));
            ptr = next;
        } while (next != 0);
    }
}

static struct hashtable_data* hashtable_find(const char* key) {
    struct hashtable_data* node = hashtable[hash_func(key) % hashtable_size];
    while (node != 0) {
        if (node->next == 0) {
            break;
        }
        node = node->next;
    }
#ifdef USE_MEMORY_DEBUG_INFO
    if (node != 0) {
        printf("  .$: 0x%016llx !  %16s :  %16s\n", (u64)node, node->key, node->value);
    }
#endif
    return node;
}

static struct hashtable_data* hashtable_get(const char* key) {
    struct hashtable_data* node = hashtable[hash_func(key) % hashtable_size];
#ifdef USE_MEMORY_DEBUG_INFO
    if (node != 0) {
        printf("  <$: 0x%016llx !  %16s :  %16s\n", (u64)node, node->key, node->value);
    }
#endif
    return node;
}

static void hashtable_set(const char* key, const char* value) {
    struct hashtable_data* node = hashtable[hash_func(key) % hashtable_size];
    if (node != 0) {
        update(&node->value, value);
    } else {
        node = hashtable_alloc(key, value);
    }
#ifdef USE_MEMORY_DEBUG_INFO
    if (node != 0) {
        printf("  >$: 0x%016llx !  %16s :  %16s\n", (u64)node, node->key, node->value);
    }
#endif
}

static void update(char** prev, const char* new) {
    if (prev != 0) {
        if (*prev != 0) {
            _list_free(*prev, 0);
            *prev = 0;
        }
        char* value = _list_alloc(strlen(new) + 1); /* +1 for ’\0’ */
        strcpy(value, new); /* NOLINT */
        *prev = value;
    }
}

/* public */

const struct hashtable hashtable_definition = {
    .init = hashtable_init,
    .destroy = hashtable_destroy,
    .alloc = hashtable_alloc,
    .free = hashtable_free,
    .find = hashtable_find,
    .get = hashtable_get,
    .set = hashtable_set,
    .setup = hashtable_setup
};
