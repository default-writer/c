#ifndef _STD_MACROS_H_
#define _STD_MACROS_H_

#define CLEAN(ptr)  \
    if (ptr != 0) { \
        ptr = 0;    \
    }

#if defined(__GNUC__)
#define FALL_THROUGH __attribute__((fallthrough))
#define ATTRIBUTE
#define INIT __attribute__((constructor))
#define DESTROY __attribute__((destructor))
#else
#define INIT
#define DESTROY
#define FALL_THROUGH ((void)0)
#endif /* __GNUC__ >= 7 */

#ifdef VM_DEBUG_INFO
#undef VM_DEBUG_INFO
#endif

#define RXP_DEBUG_TESTS

#endif /* _STD_MACROS_H_ */
