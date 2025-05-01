#define USING_VIRTUAL_PLATFORM

#ifndef VIRTUAL_PLATFORM_H
#define VIRTUAL_PLATFORM_H

#if defined(_WIN32)
#include "virtual/platform/win32/export.h"
#else
#include "virtual/platform/default/export.h"
#endif

#endif /* VIRTUAL_PLATFORM_H */
