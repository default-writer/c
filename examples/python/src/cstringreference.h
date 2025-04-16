#define USING_CSTRINGREFERENCE

#ifndef CSTRINGREFERENCE_H
#define CSTRINGREFERENCE_H

#include "macros.h"

#include "std/data.h"
#include "system/error/error_v1.h"
#include "virtual/types/string_reference/string_reference_v1.h"

int init_cstringreference(PyObject* module);

typedef struct CStringReference {
    PyObject_HEAD;
    const_vm_ptr cvm;
} CStringReferenceType;
typedef struct CStringReference* CStringReferenceTypePtr;

extern PyTypeObject CStringReferenceTypeObject;

#endif // CSTRINGREFERENCE_H
