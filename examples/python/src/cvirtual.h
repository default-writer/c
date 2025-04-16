#define USING_CVIRTUAL

#ifndef CVIRTUAL_H
#define CVIRTUAL_H

#include "macros.h"

#include "std/data.h"
#include "system/error/error_v1.h"
#include "virtual/virtual/virtual_v1.h"

int init_cvirtual(PyObject* module);

typedef struct CVirtual {
    PyObject_HEAD;
    const_vm_ptr cvm;
} CVirtualType;
typedef struct CVirtual* CVirtualTypePtr;

extern PyTypeObject CVirtualTypeObject;

#endif // CVIRTUAL_H
