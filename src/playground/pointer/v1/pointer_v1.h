#ifndef _POINTER_H_
#define _POINTER_H_

#include "std/common.h"

enum type {
    TYPE_VOID = 0,
    /* value used for pointer type */
    TYPE_PTR = 1,
    /* value used for file type */
    TYPE_FILE = 2,
    /* value used for list type */
    TYPE_LIST = 3
};

struct list_data;

/* private */
struct pointer {
#ifndef USE_GC
    struct vm_data* vm;
#endif
    void* data;
    u64 size;
    enum type type;
};

struct pointer_data {
    struct vm_data* vm;
    struct list_data* list;
    struct list_data* free;
#ifdef USE_GC
    struct list_data* gc;
#endif
};

struct pointer_vm_methods {
    void (*register_free)(void (*free)(u64 ptr));
    struct pointer* (*alloc)(u64 size, enum type type);
    void (*realloc)(struct pointer* ptr, u64 size);
    void (*free)(struct pointer* ptr);
    void (*cleanup)(struct list_data** current);
};

struct pointer_methods {
    void (*init)(u64 size);
    void (*destroy)(void);
    u64 (*alloc)(void);
    u64 (*copy)(u64 ptr);
    u64 (*peek)(void);
    void (*push)(u64 ptr);
    u64 (*pop)(void);
    void (*strcpy)(u64 dest, u64 src);
    void (*strcat)(u64 dest, u64 src);
    u64 (*match_last)(u64 src_ptr, u64 match_prt);
    u64 (*getcwd)(void);
    u64 (*load)(const char* data);
    void (*printf)(u64 ptr);
    void (*put_char)(u64 ptr, char value);
    char* (*unsafe)(u64 ptr);
    u64 (*size)(u64 ptr);
#ifdef USE_MEMORY_DEBUG_INFO
    void (*dump)(struct pointer* ptr);
    void (*dump_ref)(void** ptr);
#endif
#ifndef USE_GC
    void (*free)(u64 ptr);
#else
    void (*gc)(void);
#endif
};

void pointer_ctx_init(struct pointer_data** ctx, u64 size);
void pointer_ctx_destroy(struct pointer_data** ctx);

#endif /* _POINTER_H_ */
