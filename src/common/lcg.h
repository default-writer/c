#ifndef _COMMON_LCG_H_
#define _COMMON_LCG_H_

#include "std/common.h"

/* LCG Park-Miller function */
u32 lcg_parkmiller(u32* state);
u32 lcg_parkmiller_state(void);

#endif // _COMMON_LCG_H_
