#ifndef _COMMON_MEMORY_H_
#define _COMMON_MEMORY_H_

#include "std/common.h"

extern void (*memory_init)();
extern void (*memory_destroy)();
extern void* (*memory_alloc)(u32 nmemb, u32 size);
extern void (*memory_free)(void* ptr, u32 size);

void memory_init_v1();
void memory_init_v2();
void memory_destroy_v1();
void memory_destroy_v2();
void* memory_alloc_v1(u32 nmemb, u32 size);
void* memory_alloc_v2(u32 nmemb, u32 size);
void memory_free_v1(void* data, u32 size);
void memory_free_v2(void* data, u32 size);

#endif // _COMMON_MEMORY_H_