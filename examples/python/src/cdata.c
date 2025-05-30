/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 12, 2025 at 5:02:55 AM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cdata.h"
#include "cexception.h"
#include "cvm.h"

#include "py_api.h"

/* alloc */
static PyObject* CData_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

/* constructor/destructor */
static int CData_init(CDataTypePtr self, PyObject* args, PyObject* kwds);
static void CData_dealloc(CDataTypePtr self);

/* instance methods */
static PyObject* CData_size(CDataTypePtr self, PyObject* args);

/* static methods */
static PyObject* CData_unsafe_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CData_free_static(PyObject* cls, PyObject* args, PyObject* kwargs);

/* context manager protocol */
static PyObject* CData_enter(CDataTypePtr self, PyObject* Py_UNUSED(ignored));
static PyObject* CData_exit(CDataTypePtr self, PyObject* args);

static const char* CData_Empty = "\0";

static PyObject* CData_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CDataTypePtr self;
    self = (CDataTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
        self->ptr = 0;
    }
    return (PyObject*)self;
}

static int CData_init(CDataTypePtr self, PyObject* args, PyObject* kwds) {
    PyObject* cvm_obj;
    u64 size = 0;
    if (!PyArg_ParseTuple(args, "OK", &cvm_obj, &size)) {
        return -1;
    }

    if (!PyObject_TypeCheck(cvm_obj, &CVirtualMachineTypeObject)) {
        PYTHON_ERROR(PyExc_TypeError, "expected a CVirtualMachine instance");
        return -1;
    }

    CVirtualMachineTypePtr cvm = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer");
        return -1;
    }

    self->cvm = cvm->cvm;

    u64 ptr = PY_CALL(data)->alloc(self->cvm, size);
    if (!ptr) {
        PYTHON_ERROR(CInvalidArgumentException, "failed to allocate data block: invalid size or insufficient memory");
        return -1;
    }
    self->ptr = ptr;

    return 0;
}

static void CData_dealloc(CDataTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CData_size(CDataTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 size = PY_CALL(data)->size(self->cvm, address);
    if (!size) {
        PYTHON_ERROR(CInvalidPointerException, "failed to get size: invalid data block address");
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(size);
}

static PyObject* CData_unsafe_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!K|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance");
        return NULL;
    }

    const char* data = PY_CALL(data)->unsafe(cvm_py->cvm, src);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to get raw string data: invalid address or operation failed");
            return NULL;
        }
        CALL(error)->clear();
        data = CData_Empty;
    }

    return PyUnicode_FromString(data);
}

static PyObject* CData_free_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 address;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!K|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &address,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance");
        return NULL;
    }

    u64 result = PY_CALL(data)->free(cvm_py->cvm, address);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to free pointer: invalid pointer address: %d", address);
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CData_enter(CDataTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CData_exit(CDataTypePtr self, PyObject* args) {
    PyObject *exc_type, *exc_value, *traceback;
    if (!PyArg_ParseTuple(args, "OOO", &exc_type, &exc_value, &traceback)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyMethodDef CData_methods[] = {
    /* CData instance methods */
    { "size", (PyCFunction)CData_size, METH_VARARGS, "Get the size of a data block" },

    /* CData static methods */
    { "unsafe", (PyCFunction)CData_unsafe_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Get a pointer to the data block" },
    { "free", (PyCFunction)CData_free_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Free pointer (static method)." },

    /* CData context */
    { "__enter__", (PyCFunction)CData_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CData_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CDataTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CData",
    .tp_doc = "CData implementation in C module",
    .tp_basicsize = sizeof(CDataType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CData_new,
    .tp_init = (initproc)CData_init,
    .tp_dealloc = (destructor)CData_dealloc,
    .tp_methods = CData_methods,
};

int init_cdata(PyObject* module) {
    if (PyType_Ready(&CDataTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CDataTypeObject);
    if (PyModule_AddObject(module, "CData", (PyObject*)&CDataTypeObject) < 0) {
        Py_DECREF(&CDataTypeObject);
        return -1;
    }

    return 0;
}
