#define USING_CENVIRONMENT

#ifndef CENVIRONMENT_H
#define CENVIRONMENT_H

#include "macros.h"

#include "std/data.h"
#include "system/error/error_v1.h"
#include "virtual/env/env_v1.h"

int init_cenvironment(PyObject* module);

typedef struct CEnvironment {
    PyObject_HEAD;
    const_vm_ptr cvm;
} CEnvironmentType;
typedef struct CEnvironment* CEnvironmentTypePtr;

extern PyTypeObject CEnvironmentTypeObject;

#endif // CENVIRONMENT_H
