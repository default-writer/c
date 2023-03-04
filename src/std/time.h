#ifndef _STD_TIME_H_
#define _STD_TIME_H_

#include <sys/time.h>

#include "std/data.h"

#define TIME_START(s)     \
    struct timeval s##t1; \
    gettimeofday(&s##t1, NULL);

#define TIME_END(s)       \
    struct timeval s##t2; \
    gettimeofday(&s##t2, NULL);

#define TIME_PRINT(s)                                                                        \
    struct timeval s##diff = { s##t2.tv_sec - s##t1.tv_sec, s##t2.tv_usec - s##t1.tv_usec }; \
    u64 s##elapsed = (u64)(s##diff.tv_sec * 1000000 + s##diff.tv_usec);                      \
    printf("%0.3Lf\n", (long double)s##elapsed / 1000.0);

#endif // _STD_TIME_H_
