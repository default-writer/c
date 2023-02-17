#ifndef _STD_MACROS_H_
#define _STD_MACROS_H_

#define ZEROPTR(ptr) \
    if (ptr != 0) {  \
        ptr = 0;     \
    }

#define debug(fmt, ...) printf("%s:%d:\n" fmt, __FILE__, __LINE__, __VA_ARGS__)

#endif // _STD_MACROS_H_
