#define USING_CFILE

#ifndef CFILE_H
#define CFILE_H

#include "macros.h"

#include "std/data.h"
#include "system/error/error_v1.h"
#include "virtual/types/file/file_v1.h"

int init_cfile(PyObject* module);

typedef struct CFile {
    PyObject_HEAD;
    const_vm_ptr cvm;
} CFileType;
typedef struct CFile* CFileTypePtr;

extern PyTypeObject CFileTypeObject;

#endif // CFILE_H
