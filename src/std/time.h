#ifndef _STD_TIME_H_
#define _STD_TIME_H_

#include <sys/time.h>

#include "std/data.h"

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

#endif // _STD_TIME_H_
