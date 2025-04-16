#define USING_CDATA

#ifndef CDATA_H
#define CDATA_H

#include "macros.h"

#include "std/data.h"
#include "system/error/error_v1.h"
#include "virtual/types/data/data_v1.h"

int init_cdata(PyObject* module);

typedef struct CData {
    PyObject_HEAD;
    const_vm_ptr cvm;
} CDataType;
typedef struct CData* CDataTypePtr;

extern PyTypeObject CDataTypeObject;

#endif // CDATA_H
