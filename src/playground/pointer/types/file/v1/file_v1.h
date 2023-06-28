#ifndef _POINTER_TYPES_FILE_H_
#define _POINTER_TYPES_FILE_H_

#include "std/common.h"

struct file_methods {
    u64 (*alloc)(u64 file_path_ptr, u64 mode_ptr);
    u64 (*read)(u64 ptr);
    void (*free)(u64 ptr);
};

#endif /* _POINTER_TYPES_FILE_H_ */