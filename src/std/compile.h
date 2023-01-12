#ifndef _STD_COMPILE_H_
#define _STD_COMPILE_H_

#ifdef MEMORY_DEBUG_INFO
#if MEMORY_DEBUG_INFO == 1
#define USE_MEMORY_DEBUG_INFO
#endif
#endif

#ifdef MEMORY_CLEANUP
#if MEMORY_CLEANUP == 1
#define USE_MEMORY_CLEANUP
#endif
#endif

#ifdef MEMORY_LEAKS
#if MEMORY_LEAKS == 1
#define USE_MEMORY_LEAKS_DETECTION
#endif
#endif

#endif // _STD_COMPILE_H_
