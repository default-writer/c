#ifndef _PLAYGROUND_BRAIN_H_
#define _PLAYGROUND_BRAIN_H_

#include "std/common.h"

extern inline void process(char* data) __attribute__((always_inline));
extern inline void get_full_path(int argc, char** argv, char* path, const char* file_name) __attribute__((always_inline));
extern inline long get_file_size(FILE* f) __attribute__((always_inline));

#endif // _PLAYGROUND_BRAIN_H_