/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:15:15 GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "lcg.h"
#include "std/common.h"

static u32 lcg_parkmiller_state_internal(void);

/* LCG Park-Miller state */
static u32 state = (0x7bde8421 & 0x7fffffff) - 1; /* 0x7bde8420 */

/* LCG Park-Miller function */
static u32 lcg_parkmiller_state_internal(void) {
    u64 product = (u64)state * 48271;
    u32 x = (product & 0x7fffffff) + (product >> 31);
    x = (x & 0x7fffffff) + (x >> 31);
    return state = (u32)x;
}

u64 lcg_parkmiller_64(void) {
    return (lcg_parkmiller_state_internal() << 32) + lcg_parkmiller_state_internal();
}
