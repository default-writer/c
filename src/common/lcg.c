#include "std/common.h"

/* LCG Park-Miller state */
u64 lcg_state = (0xdeadbeef & 0x7fffffff) - 1; // 0x5eadbeee

/* LCG Park-Miller function */
u32 lcg_parkmiller() {
    lcg_state = lcg_state * 48271 % 0x7fffffff;
    return (u32)lcg_state;
}
