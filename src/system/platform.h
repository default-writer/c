#define USING_SYSTEM_PLATFORM

#ifndef SYSTEM_PLATFORM_H
#define SYSTEM_PLATFORM_H

#if defined(_WIN32)
#include "system/platform/win32/export.h"
#else
#include "system/platform/default/export.h"
#endif

#endif /* SYSTEM_PLATFORM_H */
