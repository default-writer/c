#include "common/alloc.h"

#include "common/lcg.h"
#include "playground/hashtable/hashtable.h"
#include "playground/list/v2/list.h"
#include "playground/pointer/pointer.h"

#define DEFAULT_SIZE 101

static struct hashtable_data** hashtable; /* pointer table */

static struct hashtable_data* hashtable_alloc(char* name, char* value);
static void hashtable_free(struct hashtable_data* node);
static struct hashtable_data* hashtable_extract(struct hashtable_data* head, struct hashtable_data* node);
static struct hashtable_data* hashtable_find(char* name);
static struct hashtable_data* hashtable_get(char* name, char* value);
static void hashtable_set(struct hashtable_data* node, char* name, char* value);
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
    hashtable = calloc(size, sizeof(struct hashtable_data*));
}

static void hashtable_destroy(void) {
    for (u64 i = 0; i < hashtable_size; i++) {
        hashtable_free(hashtable[i]);
    }
    free(hashtable);
    hashtable_size = 0;
}

static struct hashtable_data* hashtable_alloc(char* name, char* value) {
    struct hashtable_data* node = calloc(1, sizeof(struct hashtable_data));
    update(&node->name, name);
    update(&node->value, value);
    u32 hash = hashfunc(name);
    struct hashtable_data* next = hashtable[hash];
    node->next = next;
    hashtable[hash] = node;
    return node;
}

static void hashtable_free(struct hashtable_data* node) {
    if (node != 0) {
        struct hashtable_data* tmp = node;
        if (tmp != 0) {
            struct hashtable_data* next;
            do {
                u32 hash = hashfunc(tmp->name);
                if (hashtable[hash] != 0) {
                    struct hashtable_data* found = hashtable_extract(hashtable[hash], tmp);
                    if (hashtable[hash] != found) {
                        free(tmp->name);
                        free(tmp->value);
                        free(tmp);
                        break;
                    } else {
                        hashtable[hash] = 0;
                    }
                }
                next = tmp->next;
                free(tmp->name);
                free(tmp->value);
                free(tmp);
                tmp = next;
            } while (next != 0);
        }
    }
}

static struct hashtable_data* hashtable_extract(struct hashtable_data* head, struct hashtable_data* node) {
    struct hashtable_data* tmp = head;
    struct hashtable_data* prev = 0;
    while (tmp != 0 && tmp != node) {
        prev = tmp;
        tmp = tmp->next;
    }
    if (tmp != 0 && prev != 0) {
        prev->next = tmp->next;
    }
    return tmp;
}

static struct hashtable_data* hashtable_find(char* name) {
    struct hashtable_data* node = hashtable[hashfunc(name)];
    while (node != 0 && strcmp(name, node->name) != 0) {
        node = node->next;
    }
    return node;
}

static struct hashtable_data* hashtable_get(char* name, char* value) {
    struct hashtable_data* node = hashtable_find(name);
    if (node == 0) {
        node = hashtable_alloc(name, value);
    } else {
        hashtable_set(node, name, value);
    }
    return node;
}

static void hashtable_set(struct hashtable_data* node, char* name, char* value) {
    printf("%s %s\n", node->name, name);
    update(&node->value, value);
}

static void update(char** prev, char* new) {
    if (prev != 0) {
        if (*prev != 0) {
            free(*prev);
            *prev = 0;
        }
        char* value = calloc(1, strlen(new) + 1); /* +1 for ’\0’ */
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
