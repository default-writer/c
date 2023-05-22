#ifndef _STD_COMPILE_H_
#define _STD_COMPILE_H_

#ifdef CONFIG_MEMORY_DEBUG_INFO
#if CONFIG_MEMORY_DEBUG_INFO == 1
#define USE_MEMORY_DEBUG_INFO
#endif
#endif

#ifdef CONFIG_MEMORY_CLEANUP
#if CONFIG_MEMORY_CLEANUP == 1
#define USE_MEMORY_CLEANUP
#endif
#endif

#ifdef CONFIG_MEMORY_ALLOC
#if CONFIG_MEMORY_ALLOC == 1
#define USE_MEMORY_ALLOC
#endif
#endif

#ifdef GC
#if GC == 1
#define USE_GC
#endif
#endif

#ifdef MOCKS
#if MOCKS == 1
#define USE_MOCKS
#endif
#endif

#ifdef CONFIG_MEMCPY
#if CONFIG_MEMCPY == 1
#define USE_MEMCPY
#endif
#endif

#endif /* _STD_COMPILE_H_ */
