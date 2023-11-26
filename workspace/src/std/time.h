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
#ifndef _STD_TIME_H_
#define _STD_TIME_H_

#include "std/data.h"
#include <sys/time.h>

#define TIME_INIT(s)      \
    struct timeval s##t1; \
    struct timeval s##t2; \
    struct timeval s##diff;

#define TIME_START(s) \
    gettimeofday(&s##t1, NULL);

#define TIME_END(s)             \
    gettimeofday(&s##t2, NULL); \
    TIME_CALC(s);

#define TIME_CALC(s)                              \
    s##diff.tv_sec = s##t2.tv_sec - s##t1.tv_sec; \
    s##diff.tv_usec = s##t2.tv_usec - s##t1.tv_usec;

#define TIME_VALUE(s) \
    (u64)(s##diff.tv_sec * 1000000 + s##diff.tv_usec)

#define TIME_PRINT(s) printf("%0.3Lf\n", (long double)(TIME_VALUE(s)) / 1000.0);

#endif /* _STD_TIME_H_ */
