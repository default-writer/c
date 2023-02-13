#ifndef _PLAYGROUND_POINTER_H_
#define _PLAYGROUND_POINTER_H_

#include "std/common.h"

struct pointer;

struct pointer_methods {
    u64 (*alloc)();
    u64 (*copy)(u64 ptr);
    u64 (*peek)();
    void (*push)(u64 ptr);
    u64 (*pop)();
    void (*strcpy)(u64 dest, u64 src);
    void (*strcat)(u64 dest, u64 src);
    u64 (*match_last)(u64 src_ptr, u64 match_prt);
    u64 (*load)(const char* data);
    u64 (*open_file)(u64 file_path_ptr, u64 mode_ptr);
    u64 (*read_file)(u64 ptr);
    void (*close_file)(u64 ptr);
    void (*printf)(u64 ptr);
    void (*put_char)(u64 ptr, char value);
#ifndef USE_GC
    void (*free)(u64 ptr);
#else
    void (*gc)();
#endif
};

typedef void (*pointer_function)(u64 ptr);

void pointer_init(u64 size);
void pointer_destroy();

struct init_data {
    struct vm_data* vm;
    struct list_data* list;
#ifdef USE_GC
    struct list_data* gc_list;
#endif
};

void pointer_get(struct init_data* init);
void pointer_set(struct init_data* init);

#endif // _PLAYGROUND_POINTER_H_