#define USING_CSTRING

#ifndef CSTRING_H
#define CSTRING_H

#include "macros.h"

#include "std/data.h"
#include "system/error/error_v1.h"
#include "virtual/types/string/string_v1.h"

int init_cstring(PyObject* module);

typedef struct CString {
    PyObject_HEAD;
    const_vm_ptr cvm;
} CStringType;
typedef struct CString* CStringTypePtr;

extern PyTypeObject CStringTypeObject;

#endif // CSTRING_H
