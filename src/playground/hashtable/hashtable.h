#ifndef _PLAYGROUND_HASHTABLE_H_
#define _PLAYGROUND_HASHTABLE_H_

#include "std/common.h"

struct hashtable_data { /* table entry: */
    struct hashtable_data* next; /* next entry in chain */
    char* name; /* defined name */
    char* value; /* replacement text */
};

struct hashtable {
    void (*init)(u64 size);
    void (*destroy)(void);
    struct hashtable_data* (*alloc)(char* name, char* value);
    void (*free)(struct hashtable_data* node);
    struct hashtable_data* (*find)(char* s);
    struct hashtable_data* (*get)(char* name, char* value);
    void (*set)(struct hashtable_data* node, char* name, char* value);
};

#endif // _PLAYGROUND_HASHTABLE_H_