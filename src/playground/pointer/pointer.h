#ifndef _PLAYGROUND_POINTER_H_
#define _PLAYGROUND_POINTER_H_

#include "std/common.h"

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
    u64 (*open_file)(u64 file_path_ptr, u64 mode_ptr);
    u64 (*read_file)(u64 ptr);
    void (*close_file)(u64 ptr);
    void (*printf)(u64 ptr);
    void (*put_char)(u64 ptr, char value);
    char* (*unsafe)(u64 ptr);
    u64 (*size)(u64 ptr);
#ifndef USE_GC
    void (*free)(u64 ptr);
#else
    void (*gc)(void);
#endif
};

void pointer_get(struct pointer_data** init);
void pointer_set(struct pointer_data** init);

#endif // _PLAYGROUND_POINTER_H_
