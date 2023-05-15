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

struct pointer;

struct pointer_data;

struct pointer_methods {
    void (*init)(u64 size);
    void (*destroy)(void);
    u64 (*list_alloc)(void);
    void (*list_free)(u64);
    u64 (*list_peek)(u64 list_ptr);
    u64 (*list_pop)(u64 list_ptr);
    void (*list_push)(u64 list_ptr, u64 ptr);
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
    u64 (*file_alloc)(u64 file_path_ptr, u64 mode_ptr);
    u64 (*file_read)(u64 ptr);
    void (*file_free)(u64 ptr);
    void (*printf)(u64 ptr);
    void (*put_char)(u64 ptr, char value);
    char* (*unsafe)(u64 ptr);
    u64 (*size)(u64 ptr);
#ifdef USE_MEMORY_DEBUG_INFO
    void (*dump)(struct pointer* ptr);
    void (*dump_ref)(struct pointer* ptr);
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
