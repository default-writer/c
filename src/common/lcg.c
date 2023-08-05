/*
 *
 * MIT License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include "lcg.h"
#include "std/common.h"

static u32 lcg_parkmiller_state_internal(void);

/* LCG Park-Miller state */
static u64 lcg_state = (0xdeadbeef & 0x7fffffff) - 1; /* 0x5eadbeee */

/* LCG Park-Miller function */
static u32 lcg_parkmiller_state_internal(void) {
    lcg_state = lcg_state * 48271 % 0x7fffffff;
    return (u32)lcg_state;
}

u64 lcg_parkmiller_64(void) {
    return (u64)(((u64)(lcg_parkmiller_state_internal()) << 32) + lcg_parkmiller_state_internal());
}
