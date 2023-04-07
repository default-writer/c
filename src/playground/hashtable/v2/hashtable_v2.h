#ifndef _PLAYGROUND_HASHTABLE_V2_H_
#define _PLAYGROUND_HASHTABLE_V2_H_

#define HASHTABLE_DEFAULT_SIZE 101

#include "std/common.h"

struct hashtable_data { /* table entry: */
    struct hashtable_data* next; /* next entry in chain */
    char* key; /* defined name */
    // char* value; /* replacement text */
    void* value;
};

struct hashtable {
    void (*init)(u64 size);
    void (*destroy)(void);
    struct hashtable_data* (*alloc)(char* key, void* value);
    void (*free)(struct hashtable_data* node);
    struct hashtable_data* (*find)(char* key);
    struct hashtable_data* (*value)(u32 index);
    u32 (*get)(char* key);
    u32 (*hash)(char* key);
    u32 (*count)(char* key);
    void (*set)(char* key, void* value);
    void (*setup)(u32 (*function)(char* source));
};

u32 default_hash(char* source);
u32 murmurhash3(char* source);

#endif // _PLAYGROUND_HASHTABLE_V2_H_
