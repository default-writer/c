/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   11 December 2023 at 9:06:14 GMT+3
 * Modified:
 *   11 December 2023 at 9:16:31 GMT+3
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
