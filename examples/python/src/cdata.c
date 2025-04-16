/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 16, 2025 at 11:03:49 AM GMT+3
 * Modified:
 *   April 16, 2025 at 6:31:35 PM GMT+3
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
#include "cvm.h"
#include "cexception.h"

static PyObject* CData_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CDataTypePtr self;
    self = (CDataTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
    }
    return (PyObject*)self;
}

static int CData_init(CDataTypePtr self, PyObject* args, PyObject* kwds) {
    PyObject* cvm_obj;
    if (!PyArg_ParseTuple(args, "O", &cvm_obj)) {
        return -1;
    }

    if (!PyObject_TypeCheck(cvm_obj, &CVirtualMachineTypeObject)) {
        PyErr_SetString(PyExc_TypeError, "Expected a CVirtualMachine instance");
        return -1;
    }

    CVirtualMachineTypePtr cvm = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm->cvm == NULL) {
        PyErr_SetString(CVirtualMachineNotInitializedException, "Invalid CVirtualMachine pointer");
        return -1;
    }
    self->cvm = cvm->cvm;

    return 0;
}

static void CData_dealloc(CDataTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CData_alloc(CDataTypePtr self, PyObject* args) {
    u64 size;
    if (!PyArg_ParseTuple(args, "K", &size)) {
        return NULL;
    }

    u64 address = CALL(data)->alloc(self->cvm, size);
    if (!address) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(address);
}

static PyObject* CData_size(CDataTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 size = CALL(data)->size(self->cvm, address);
    if (!size) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(size);
}

static PyObject* CData_unsafe(CDataTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    void_ptr data = CALL(data)->unsafe(self->cvm, address);
    if (data == NULL) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromVoidPtr(data);
}

static PyObject* CData_free(CDataTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 result = CALL(data)->free(self->cvm, address);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CData_enter(CDataTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CData_exit(CDataTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CData_methods[] = {
    { "alloc", (PyCFunction)CData_alloc, METH_VARARGS, "Allocate a data block" },
    { "size", (PyCFunction)CData_size, METH_VARARGS, "Get the size of a data block" },
    { "unsafe", (PyCFunction)CData_unsafe, METH_VARARGS, "Get a pointer to the data block" },
    { "free", (PyCFunction)CData_free, METH_VARARGS, "Free a data block" },
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
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
