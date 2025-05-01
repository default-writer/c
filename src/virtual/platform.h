#ifndef VIRTUAL_PLATFORM_H
#define VIRTUAL_PLATFORM_H

#if defined(__MSYS__) 
    #include "virtual/platform/msys/export.h"
#elif defined(_WIN32)
    #include "virtual/platform/win32/export.h"
#endif

#endif /* VIRTUAL_PLATFORM_H */
