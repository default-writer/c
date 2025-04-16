#define USING_CPOINTER

#ifndef CPOINTER_H
#define CPOINTER_H

#include "macros.h"

#include "std/data.h"
#include "system/error/error_v1.h"
#include "virtual/pointer/pointer_v1.h"

int init_cpointer(PyObject* module);

typedef struct CPointer {
    PyObject_HEAD;
    const_vm_ptr cvm;
} CPointerType;
typedef struct CPointer* CPointerTypePtr;

extern PyTypeObject CPointerTypeObject;

#endif // CPOINTER_H
