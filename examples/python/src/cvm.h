#define USING_CVM

#ifndef CVM_H
#define CVM_H

#include "macros.h"

#include "std/data.h"
#include "virtual/vm/vm_v1.h"

int init_cvm(PyObject* module);

typedef struct CVirtualMachine {
    PyObject_HEAD;
    const_vm_ptr cvm;
} CVirtualMachineType;
typedef struct CVirtualMachine* CVirtualMachineTypePtr;

extern PyTypeObject CVirtualMachineTypeObject;

#endif // CVM_H
