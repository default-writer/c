#include "common/alloc.h"

#include "common/lcg.h"
#include "playground/hashtable/hashtable.h"
#include "playground/list/v2/list.h"
#include "playground/pointer/pointer.h"

#define DEFAULT_SIZE 101

static struct hashtable_data* hashtable_extract_internal(struct hashtable_data* head, struct hashtable_data* ptr);

static struct hashtable_data** hashtable; /* pointer table */

static struct hashtable_data* hashtable_alloc(char* key, char* value);
static void hashtable_free(struct hashtable_data* node);
static struct hashtable_data* hashtable_find(char* key);
static struct hashtable_data* hashtable_get(char* key);
static void hashtable_set(char* key, char* value);
static void update(char** prev, char* new);

static void hashtable_init(u64 size);
static void hashtable_destroy(void);

extern u32 lcg_state;

static u64 hashtable_size = DEFAULT_SIZE;

/* hash: form hash value for string s */
static u32 hashfunc(char* source) {
    u32 data = 0;
    if (source != 0) {
        u32 hash = 0;
        char* ptr = source;
        u32 p = (u8)*ptr;
        while (*ptr != 0) {
            u16 p0 = (u16)(p & 0xff);
            u16 p1 = (u16)((p & 0xff00) >> 8);
            u16 p2 = (u16)((p & 0xff0000) >> 16);
            u16 p3 = (u16)((p & 0xff000000) >> 24);
            u16 p4 = (u8)*ptr;
            p3 = (u16)((p4 ^ p3) + (p2 | ~p4));
            p2 = (u16)((p3 ^ p2) + (p1 | ~p3));
            p1 = (u16)((p2 ^ p1) + (p0 | ~p2));
            p0 = (u16)((p1 ^ p0) + (p3 | ~p1));
            hash = (u32)(p4 + (p3 << 1) + (p2 << 3) + (p1 << 5) + (p0 << 7));
            ptr++;
        }
        data = hash % hashtable_size;
    }
    return data;
}

static void hashtable_init(u64 size) {
    hashtable_size = size;
    hashtable = _list_alloc(hashtable_size * sizeof(void*));
}

static void hashtable_destroy(void) {
    for (u64 i = 0; i < hashtable_size; i++) {
        hashtable_free(hashtable[i]);
    }
    _list_free(hashtable, hashtable_size * sizeof(void*));
    hashtable_size = 0;
}

static struct hashtable_data* hashtable_extract_internal(struct hashtable_data* head, struct hashtable_data* ptr) {
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

static struct hashtable_data* hashtable_alloc(char* key, char* value) {
    struct hashtable_data* node = _list_alloc(sizeof(struct hashtable_data));
    update(&node->key, key);
    update(&node->value, value);
    u32 hash = hashfunc(key);
    struct hashtable_data* next = hashtable[hash];
    node->next = next;
    hashtable[hash] = node;
    return node;
}

static void hashtable_free(struct hashtable_data* node) {
    if (node != 0) {
        struct hashtable_data* ptr = node;
        if (ptr != 0) {
            struct hashtable_data* next;
            do {
                u32 hash = hashfunc(ptr->key);
                if (hashtable[hash] != 0) {
                    struct hashtable_data* found = hashtable_extract_internal(hashtable[hash], ptr);
                    if (hashtable[hash] != found) {
                        _list_free(ptr->key, 0);
                        _list_free(ptr->value, 0);
                        _list_free(ptr, sizeof(struct hashtable_data));
                        break;
                    } else {
                        hashtable[hash] = 0;
                    }
                }
                next = ptr->next;
                _list_free(ptr->key, 0);
                _list_free(ptr->value, 0);
                _list_free(ptr, sizeof(struct hashtable_data));
                ptr = next;
            } while (next != 0);
        }
    }
}

static struct hashtable_data* hashtable_find(char* key) {
    struct hashtable_data* node = hashtable[hashfunc(key)];
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

static struct hashtable_data* hashtable_get(char* key) {
    struct hashtable_data* node = hashtable[hashfunc(key)];
#ifdef USE_MEMORY_DEBUG_INFO
    if (node != 0) {
        printf("  <$: 0x%016llx !  %16s :  %16s\n", (u64)node, node->key, node->value);
    }
#endif
    return node;
}

static void hashtable_set(char* key, char* value) {
    struct hashtable_data* node = hashtable[hashfunc(key)];
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

static void update(char** prev, char* new) {
    if (prev != 0) {
        if (*prev != 0) {
            _list_free(*prev, 0);
            *prev = 0;
        }
        char* value = _list_alloc(strlen(new) + 1); /* +1 for ’\0’ */
        strcpy(value, new); // NOLINT
        *prev = value;
    }
}

const struct hashtable hashtable_definition = {
    .init = hashtable_init,
    .destroy = hashtable_destroy,
    .alloc = hashtable_alloc,
    .free = hashtable_free,
    .find = hashtable_find,
    .get = hashtable_get,
    .set = hashtable_set
};
