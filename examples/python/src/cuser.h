#define USING_CUSER

#ifndef CUSER_H
#define CUSER_H

#include "macros.h"

#include "std/data.h"
#include "system/error/error_v1.h"
#include "virtual/types/user/user_v1.h"

int init_cuser(PyObject* module);

typedef struct CUser {
    PyObject_HEAD;
    u64 user_ptr;
    const_vm_ptr cvm;
} CUserType;
typedef struct CUser* CUserTypePtr;

extern PyTypeObject CUserTypeObject;

#endif // CUSER_H
