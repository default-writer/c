#define USING_CLIST

#ifndef CLIST_H
#define CLIST_H

#include "macros.h"

#include "std/data.h"
#include "system/error/error_v1.h"
#include "virtual/list/list_v1.h"

int init_clist(PyObject* module);

typedef struct CList {
    PyObject_HEAD;
    stack_ptr stack;
    const_vm_ptr cvm;
} CListType;
typedef struct CList* CListTypePtr;

extern PyTypeObject CListTypeObject;

#endif // CLIST_H
