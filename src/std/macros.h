#ifndef _STD_MACROS_H_
#define _STD_MACROS_H_

#define CLEAN(ptr)  \
    if (ptr != 0) { \
        ptr = 0;    \
    }

#ifdef DEBUG

#define debug(fmt, ...) printf("%s:%d:\n" fmt, __FILE__, __LINE__, __VA_ARGS__)

#endif

#endif // _STD_MACROS_H_
