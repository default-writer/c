#define USING_CEXCEPTION

#ifndef CEXCEPTION_H
#define CEXCEPTION_H

#include "macros.h"

#include "system/error/error_v1.h"

int init_cexception(PyObject* module);

typedef struct CException {
    PyObject_HEAD;
} CExceptionType;
typedef struct CExceptionType* CExceptionTypePtr;

extern PyTypeObject CExceptionTypeObject;

extern PyObject* CException;
extern PyObject* CVirtualMachineNotInitializedException;
extern PyObject* CInvalidPointerException;
extern PyObject* CInvalidArgumentException;
extern PyObject* CInvalidTypeIdException;
extern PyObject* CInvalidValueException;

#endif // CEXCEPTION_H
