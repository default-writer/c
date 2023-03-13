#include "std/common.h"

#include "lcg.h"

/* LCG Park-Miller state */
static u64 lcg_state = (0xdeadbeef & 0x7fffffff) - 1; // 0x5eadbeee

/* LCG Park-Miller function */
static u32 lcg_parkmiller_state(void) {
    lcg_state = lcg_state * 48271 % 0x7fffffff;
    return (u32)lcg_state;
}

u64 lcg_parkmiller_64(void) {
    return (u64)((lcg_parkmiller_state() << 32) + lcg_parkmiller_state());
}

u32 lcg_parkmiller_32(void) {
    return lcg_parkmiller_state();
}
