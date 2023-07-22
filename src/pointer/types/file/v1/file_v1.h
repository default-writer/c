#ifndef _POINTER_TYPES_FILE_H_
#define _POINTER_TYPES_FILE_H_

#include "pointer/types/data/v1/data_v1.h"
#include "std/common.h"

struct file_methods {
    u64 (*alloc)(u64 file_path_ptr, u64 mode_ptr);
    u64 (*data)(u64 ptr);
    u64 (*free)(u64 ptr);
};

#endif /* _POINTER_TYPES_FILE_H_ */