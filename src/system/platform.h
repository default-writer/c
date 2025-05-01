#ifndef SYSTEM_PLATFORM_H
#define SYSTEM_PLATFORM_H

#if defined(__MSYS__) 
    #include "system/platform/msys/export.h"
#elif defined(_WIN32)
    #include "system/platform/win32/export.h"
#endif

#endif /* SYSTEM_PLATFORM_H */
