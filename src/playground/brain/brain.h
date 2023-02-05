#ifndef _PLAYGROUND_BRAIN_H_
#define _PLAYGROUND_BRAIN_H_

#include "playground/pointer/pointer.h"

extern inline void process() __attribute__((always_inline));
extern inline struct pointer* get_full_path() __attribute__((always_inline));
extern inline u64 get_file_size() __attribute__((always_inline));

#endif // _PLAYGROUND_BRAIN_H_