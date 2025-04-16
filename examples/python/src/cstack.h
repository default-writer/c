#define USING_CSTACK

#ifndef CSTACK_H
#define CSTACK_H

#include "macros.h"

#include "std/data.h"
#include "system/error/error_v1.h"
#include "virtual/types/stack/stack_v1.h"

int init_cstack(PyObject* module);

typedef struct CStack {
    PyObject_HEAD;
    u64 stack_ptr;
    const_vm_ptr cvm;
} CStackType;
typedef struct CStack* CStackTypePtr;

extern PyTypeObject CStackTypeObject;

#endif // CSTACK_H
