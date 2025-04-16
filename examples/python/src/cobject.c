/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 16, 2025 at 11:03:49 AM GMT+3
 * Modified:
 *   April 16, 2025 at 6:38:32 PM GMT+3
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

#include "cobject.h"
#include "cvm.h"
#include "cexception.h"

static PyObject* CObject_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CObjectTypePtr self;
    self = (CObjectTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
    }
    return (PyObject*)self;
}

static int CObject_init(CObjectTypePtr self, PyObject* args, PyObject* kwds) {
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

static void CObject_dealloc(CObjectTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CObject_alloc(CObjectTypePtr self, PyObject* args) {
    u64 size;
    if (!PyArg_ParseTuple(args, "K", &size)) {
        return NULL;
    }

    u64 address = CALL(object)->alloc(self->cvm, size);
    if (!address) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(address);
}

static PyObject* CObject_free(CObjectTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 result = CALL(object)->free(self->cvm, address);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CObject_unsafe(CObjectTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    void_ptr data = CALL(object)->unsafe(self->cvm, address);
    if (data == NULL) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromVoidPtr(data);
}

static PyObject* CObject_load(CObjectTypePtr self, PyObject* args) {
    const_void_ptr data;
    u64 size;
    if (!PyArg_ParseTuple(args, "Ks", &data, &size)) {
        return NULL;
    }

    u64 address = CALL(object)->load(self->cvm, data, size);
    if (!address) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(address);
}

static PyObject* CObject_size(CObjectTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 size = CALL(object)->size(self->cvm, address);
    if (!size) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(size);
}

static PyObject* CObject_enter(CObjectTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CObject_exit(CObjectTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CObject_methods[] = {
    { "alloc", (PyCFunction)CObject_alloc, METH_VARARGS, "Allocate an object" },
    { "free", (PyCFunction)CObject_free, METH_VARARGS, "Free an object" },
    { "unsafe", (PyCFunction)CObject_unsafe, METH_VARARGS, "Get a pointer to the object data" },
    { "load", (PyCFunction)CObject_load, METH_VARARGS, "Load data into an object" },
    { "size", (PyCFunction)CObject_size, METH_VARARGS, "Get the size of an object" },
    { "__enter__", (PyCFunction)CObject_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CObject_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CObjectTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CObject",
    .tp_doc = "CObject implementation in C module",
    .tp_basicsize = sizeof(CObjectType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CObject_new,
    .tp_init = (initproc)CObject_init,
    .tp_dealloc = (destructor)CObject_dealloc,
    .tp_methods = CObject_methods,
};

int init_cobject(PyObject* module) {
    if (PyType_Ready(&CObjectTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CObjectTypeObject);
    if (PyModule_AddObject(module, "CObject", (PyObject*)&CObjectTypeObject) < 0) {
        Py_DECREF(&CObjectTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
