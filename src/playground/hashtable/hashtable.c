#include "common/alloc.h"

#include "common/lcg.h"
#include "playground/hashtable/hashtable.h"
#include "playground/list/v2/list.h"
#include "playground/pointer/pointer.h"

#define DEFAULT_SIZE 101

static char* copy(char*);
static u32 hashfunc(char* s);
static struct hashtable_data** hashtable; /* pointer table */
static struct hashtable_data* hashtable_find(char* s);
static struct hashtable_data* hashtable_get(char* name, char* value);
static struct hashtable_data* hashtable_alloc(char* name, char* value);
static void hashtable_set(struct hashtable_data* node, char* name, char* value);

static void hashtable_init(u64 size);
static void hashtable_destroy(void);

extern u32 lcg_state;

/* hash: form hash value for string s */
static u32 hashfunc(char* s) {
    if (s != 0) {
        u32 hash = 0;
        u32 p = (u8)*s;
        while (*s != 0) {

            u16 p0 = (u16)(p & 0xff);
            u16 p1 = (u16)((p & 0xff00) >> 8);
            u16 p2 = (u16)((p & 0xff0000) >> 16);
            u16 p3 = (u16)((p & 0xff000000) >> 24);

            u16 p4 = (u8)*s;

            p3 = (u16)((p4 ^ p3) + (p2 | ~p4));
            p2 = (u16)((p3 ^ p2) + (p1 | ~p3));
            p1 = (u16)((p2 ^ p1) + (p0 | ~p2));
            p0 = (u16)((p1 ^ p0) + (p3 | ~p1));

            hash = (u32)(p4 + (p3 << 1) + (p2 << 3) + (p1 << 5) + (p0 << 7));
            s++;
        }
        return hash % DEFAULT_SIZE;
    }
    return 0;
}

static void hashtable_init(u64 size) {
    hashtable = calloc(size, sizeof(struct hashtable_data*));
}

static void hashtable_destroy(void) {
    free(hashtable);
}

static struct hashtable_data* hashtable_alloc(char* name, char* value) {
    struct hashtable_data* node = calloc(1, sizeof(struct hashtable_data));
    u32 hash = hashfunc(name);
    node->name = copy(name);
    node->value = copy(value);
    node->next = hashtable[hash];
    hashtable[hash] = node;
    return node;
}

static void hashtable_free(struct hashtable_data* node) {
    if (node != 0) {
        u32 hash = hashfunc(node->name);
        hashtable[hash] = 0;
        struct hashtable_data* tmp;
        while (node != 0) {
            tmp = node;
            node = node->next;
            free(tmp);
        }
    }
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
    free(node->value); /*free previous value */
    node->value = copy(value);
}

static char* copy(char* s) /* make a duplicate of s */
{
    char* p = calloc(1, strlen(s) + 1); /* +1 for ’\0’ */
    strcpy(p, s); // NOLINT
    return p;
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
