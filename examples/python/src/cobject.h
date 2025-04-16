#define USING_COBJECT

#ifndef COBJECT_H
#define COBJECT_H

#include "macros.h"

#include "std/data.h"
#include "system/error/error_v1.h"
#include "virtual/types/object/object_v1.h"

int init_cobject(PyObject* module);

typedef struct CObject {
    PyObject_HEAD;
    const_vm_ptr cvm;
} CObjectType;
typedef struct CObject* CObjectTypePtr;

extern PyTypeObject CObjectTypeObject;

#endif // COBJECT_H
