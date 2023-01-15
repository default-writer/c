#include "common/memory.h"
#include "std/common.h"
#include <stdlib.h>

#define MAX_MEMORY 0xfffffff // 256M bytes

// global allocated memory
static void* memory = 0;

/*
    Memory allocation works as follows:

    1. 8 bytes addressed by pointer should be equals to the address itself being referenced
*/
/*
    |s|_|_|_|s|_|_|_|_|_|_|s|_|_|_|

      start address >|         end address >|
                     |                      |
    +----------------+----------------------+
    |< service area >|< addressable memory >|
    +---------------+-----------------------+
    |< allocated space                     >|
    +---------------------------------------+

*/

void memory_init() {
    if (memory == 0) {
        memory = calloc(1, MAX_MEMORY);
    }
}

void memory_destroy() {
    if (memory != 0) {
        free(memory);
        memory = 0;
    }
}

void* memory_alloc(u32 nmemb, u32 size) {
    if (memory == 0) {
        return calloc(nmemb, size);
    }
    return 0;
}

void memory_free(void* ptr, u32 size) {
    if (memory == 0) {
        free(ptr);
    }
    /* releases global memory */
}