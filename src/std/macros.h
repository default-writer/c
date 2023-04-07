#ifndef _STD_MACROS_H_
#define _STD_MACROS_H_

#define CLEAN(ptr)  \
    if (ptr != 0) { \
        ptr = 0;    \
    }

#if defined(__GNUC__) && __GNUC__ >= 7
#define FALL_THROUGH __attribute__((fallthrough))
#else
#define FALL_THROUGH ((void)0)
#endif /* __GNUC__ >= 7 */

#endif /* _STD_MACROS_H_ */
