/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 16, 2025 at 11:03:49 AM GMT+3
 * Modified:
 *   April 16, 2025 at 6:38:51 PM GMT+3
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

#include "cstring.h"
#include "cvm.h"
#include "cexception.h"

static PyObject* CString_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CStringTypePtr self;
    self = (CStringTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
    }
    return (PyObject*)self;
}

static int CString_init(CStringTypePtr self, PyObject* args, PyObject* kwds) {
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

static void CString_dealloc(CStringTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CString_load(CStringTypePtr self, PyObject* args) {
    const char* data;
    if (!PyArg_ParseTuple(args, "s", &data)) {
        return NULL;
    }

    u64 address = CALL(string)->load(self->cvm, data);
    if (!address) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(address);
}

static PyObject* CString_free(CStringTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 result = CALL(string)->free(self->cvm, address);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CString_size(CStringTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 size = CALL(string)->size(self->cvm, address);
    if (!size) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(size);
}

static PyObject* CString_unsafe(CStringTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    const char* data = CALL(string)->unsafe(self->cvm, address);
    if (data == NULL) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyUnicode_FromString(data);
}

static PyObject* CString_enter(CStringTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CString_exit(CStringTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CString_methods[] = {
    { "load", (PyCFunction)CString_load, METH_VARARGS, "Load a string into the virtual machine" },
    { "free", (PyCFunction)CString_free, METH_VARARGS, "Free a string in the virtual machine" },
    { "size", (PyCFunction)CString_size, METH_VARARGS, "Get the size of a string" },
    { "unsafe", (PyCFunction)CString_unsafe, METH_VARARGS, "Get the raw string data" },
    { "__enter__", (PyCFunction)CString_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CString_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CStringTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CString",
    .tp_doc = "CString implementation in C module",
    .tp_basicsize = sizeof(CStringType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CString_new,
    .tp_init = (initproc)CString_init,
    .tp_dealloc = (destructor)CString_dealloc,
    .tp_methods = CString_methods,
};

int init_cstring(PyObject* module) {
    if (PyType_Ready(&CStringTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CStringTypeObject);
    if (PyModule_AddObject(module, "CString", (PyObject*)&CStringTypeObject) < 0) {
        Py_DECREF(&CStringTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
