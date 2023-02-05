#include "std/common.h"

/* defines N-element collection*/
#define N_ELEMENTS 2048

struct list_parameters {
    u64 block_size;
};

struct list_parameters list_parameters_definition = {
    .block_size = N_ELEMENTS
};
