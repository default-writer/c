#ifndef _PLAYGROUND_HASHTABLE_H_
#define _PLAYGROUND_HASHTABLE_H_

#include "std/common.h"

struct hashtable_data { /* table entry: */
    struct hashtable_data* next; /* next entry in chain */
    char* key; /* defined name */
    char* value; /* replacement text */
};

struct hashtable {
    void (*init)(u64 size);
    void (*destroy)(void);
    struct hashtable_data* (*alloc)(const char* key, const char* value);
    void (*free)(struct hashtable_data* node);
    struct hashtable_data* (*find)(const char* key);
    struct hashtable_data* (*get)(const char* key);
    void (*set)(const char* key, const char* value);
    void (*setup)(u32 (*function)(const char* source));
};

u32 murmurhash3(const char* source);

#endif /* _PLAYGROUND_HASHTABLE_H_ */
