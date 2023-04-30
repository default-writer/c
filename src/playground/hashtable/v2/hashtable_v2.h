#ifndef _PLAYGROUND_HASHTABLE_V2_H_
#define _PLAYGROUND_HASHTABLE_V2_H_

#define HASHTABLE_DEFAULT_SIZE 101

#include "std/common.h"

struct hashtable_data { /* table entry: */
    struct hashtable_data* next; /* next entry in chain */
    char* key; /* defined name */
    /* char* value; replacement text */
    void* value;
};

struct hashtable {
    void (*init)(u64 size);
    void (*destroy)(void);
    struct hashtable_data* (*alloc)(const char* key, void* value);
    void (*free)(struct hashtable_data* node);
    struct hashtable_data* (*find)(const char* key);
    struct hashtable_data* (*value)(u32 index);
    u32 (*get)(const char* key);
    u32 (*hash)(const char* key);
    u32 (*count)(const char* key);
    void (*set)(const char* key, void* value);
    void (*setup)(u32 (*function)(const char* source));
};

u32 default_hash(const char* source);
u32 murmurhash3(const char* source);

#endif /* _PLAYGROUND_HASHTABLE_V2_H_ */
