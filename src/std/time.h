/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 30, 2025 at 10:24:04 AM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define USING_TIME

#ifndef STD_TIME_H
#define STD_TIME_H

#define TIME_INIT(s)       \
    struct timespec s##t1; \
    struct timespec s##t2; \
    struct timespec s##diff;

#define TIME_START(s) \
    clock_gettime(CLOCK_MONOTONIC, &s##t1);

#define TIME_END(s)                         \
    clock_gettime(CLOCK_MONOTONIC, &s##t2); \
    TIME_CALC(s);

#define TIME_CALC(s)                                 \
    s##diff.tv_sec = s##t2.tv_sec - s##t1.tv_sec;    \
    s##diff.tv_nsec = s##t2.tv_nsec - s##t1.tv_nsec; \
    if (s##diff.tv_nsec < 0) {                       \
        s##diff.tv_sec -= 1;                         \
        s##diff.tv_nsec += 1000000000L;              \
    }

#define TIME_VALUE(s) \
    (u64)(s##diff.tv_sec * 1000000000ULL + s##diff.tv_nsec)

#define TIME_PRINT(s)                                                                                       \
    do {                                                                                                    \
        u64 total_ns = TIME_VALUE(s); /* Total time in nanoseconds */                                       \
        u64 hours = total_ns / (1000000000ULL * 60 * 60);                                                   \
        total_ns %= (1000000000ULL * 60 * 60);                                                              \
        u64 minutes = total_ns / (1000000000ULL * 60);                                                      \
        total_ns %= (1000000000ULL * 60);                                                                   \
        u64 seconds = total_ns / 1000000000ULL;                                                             \
        u64 milliseconds = (total_ns % 1000000000ULL) / 1000000ULL;                                         \
        u64 nanoseconds = (total_ns % 1000000ULL);                                                          \
        printf("%02llu:%02llu:%02llu.%03llu.%06llu\n", hours, minutes, seconds, milliseconds, nanoseconds); \
    } while (0)

#endif /* STD_TIME_H */
