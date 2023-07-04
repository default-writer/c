#ifndef _POINTER_H_
#define _POINTER_H_

#include "std/common.h"

enum type {
    /* value used for void type */
    TYPE_VOID = 0,
    /* value used for pointer type */
    TYPE_PTR = 1,
    /* value used for string type */
    TYPE_STRING = 2,
    /* value used for file type */
    TYPE_FILE = 3,
    /* value used for list type */
    TYPE_LIST = 4
};

struct list_data;

/* private */
struct pointer {
    struct vm_data* vm;
    void* data;
    u64 size;
    enum type type;
};

struct pointer_data {
    struct list_data* list;
    struct list_data* free;
#ifdef USE_GC
    struct list_data* gc;
#endif
};

struct pointer_vm_methods {
    struct pointer* (*alloc)(u64 size, enum type type);
    void (*realloc)(struct pointer* ptr, u64 size);
    void (*free)(struct pointer* ptr);
    void (*cleanup)(struct list_data** current);
};

struct pointer_methods {
    void (*init)(u64 size);
    void (*destroy)(void);
    u64 (*peek)(void);
    void (*push)(u64 ptr);
    u64 (*pop)(void);
    u64 (*size)(u64 ptr);
#ifdef USE_MEMORY_DEBUG_INFO
    void (*dump)(struct pointer* ptr);
    void (*dump_ref)(void** ptr);
#endif
#ifdef USE_GC
    void (*gc)(void);
#endif
};

#endif /* _POINTER_H_ */
