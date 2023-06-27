#ifndef _POINTER_TYPES_STRING_H_
#define _POINTER_TYPES_STRING_H_

#include "std/common.h"

struct string_methods {
    u64 (*alloc)(void);
    u64 (*copy)(u64 ptr);
    void (*strcpy)(u64 dest, u64 src);
    void (*strcat)(u64 dest, u64 src);
    u64 (*match_last)(u64 src_ptr, u64 match_prt);
    u64 (*getcwd)(void);
    u64 (*load)(const char* data);
    void (*printf)(u64 ptr);
    void (*put_char)(u64 ptr, char value);
    char* (*unsafe)(u64 ptr);
#ifndef USE_GC
    void (*free)(u64 ptr);
#endif
};

#endif /* _POINTER_TYPES_STRING_H_ */