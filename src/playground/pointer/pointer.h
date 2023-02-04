#ifndef _PLAYGROUND_POINTER_H_
#define _PLAYGROUND_POINTER_H_

#include "std/common.h"

struct pointer;

struct pointer_methods {
    struct pointer* (*new)(u32 size);
    struct pointer* (*new_file)(FILE* file);
    void (*delete)(struct pointer* ptr);
    void (*push)(struct pointer* ptr);
    struct pointer* (*peek)();
    struct pointer* (*pop)();
    void (*push_void)(void* ptr);
    void* (*peek_void)();
    void* (*pop_void)();
    void* (*data)(struct pointer* ptr);
    u32 (*size)(struct pointer* ptr);
    void (*strcpy)(struct pointer* dest, struct pointer* src);
    void (*strcat)(struct pointer* dest, struct pointer* src);
    struct pointer* (*match_last)(struct pointer* src_ptr, struct pointer* match_prt);
};

typedef void (*pointer_function)(struct pointer* ptr);

void pointer_init();
void pointer_destroy();

#endif // _PLAYGROUND_POINTER_H_