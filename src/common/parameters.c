#include "std/common.h"

/* defines N-element collection*/
#define N_ELEMENTS 2048
/* buffer size in bytes = size of 8 items */
#define ALLOC_SIZE(n) ((n) + 1) * sizeof(void*)

struct list_parameters {
    u32 block_size;
};

struct list_parameters list_parameters_definition = {
    .block_size = N_ELEMENTS
};
