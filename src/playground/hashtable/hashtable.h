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
    struct hashtable_data* (*alloc)(char* key, char* value);
    void (*free)(struct hashtable_data* node);
    struct hashtable_data* (*find)(char* key);
    struct hashtable_data* (*get)(char* key);
    void (*set)(char* key, char* value);
    void (*setup)(u32 (*function)(char* source));
};

u32 murmurhash3(char* source);

#endif // _PLAYGROUND_HASHTABLE_H_
