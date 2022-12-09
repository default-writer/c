#ifndef _API_H_
#define _API_H_

#if MEMORY_DEBUG_INFO==1
#define USE_MEMORY_DEBUG_INFO
#endif

#if MEMORY_CLEANUP==1
#define USE_MEMORY_CLEANUP
#endif

#if MEMORY_LEAKS==1
#define USE_MEMORY_LEAKS_DETECTION
#endif

struct list_data;

typedef long long unsigned int ADDR;
typedef unsigned char byte;

#endif // _API_H_
