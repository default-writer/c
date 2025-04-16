/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 16, 2025 at 11:03:49 AM GMT+3
 * Modified:
 *   April 16, 2025 at 6:38:39 PM GMT+3
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

#include "cpointer.h"
#include "cvm.h"
#include "cexception.h"

static PyObject* CPointer_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CPointerTypePtr self;
    self = (CPointerTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
    }
    return (PyObject*)self;
}

static int CPointer_init(CPointerTypePtr self, PyObject* args, PyObject* kwds) {
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

static void CPointer_dealloc(CPointerTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CPointer_alloc(CPointerTypePtr self, PyObject* args) {
    void_ptr data;
    u64 size, type_id;
    if (!PyArg_ParseTuple(args, "KkK", &data, &size, &type_id)) {
        return NULL;
    }

    u64 address = CALL(pointer)->alloc(self->cvm, data, size, type_id);
    if (!address) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(address);
}

static PyObject* CPointer_copy(CPointerTypePtr self, PyObject* args) {
    u64 src, size, offset, type_id;
    if (!PyArg_ParseTuple(args, "KKKK", &src, &size, &offset, &type_id)) {
        return NULL;
    }

    u64 address = CALL(pointer)->copy(self->cvm, (const_void_ptr)src, size, offset, type_id);
    if (!address) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(address);
}

static PyObject* CPointer_read(CPointerTypePtr self, PyObject* args) {
    u64 address, type_id;
    if (!PyArg_ParseTuple(args, "KK", &address, &type_id)) {
        return NULL;
    }

    const_void_ptr data = CALL(pointer)->read(self->cvm, address, type_id);
    if (data == NULL) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromVoidPtr((void_ptr)data);
}

static PyObject* CPointer_ref(CPointerTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 result = CALL(pointer)->ref(self->cvm, address);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CPointer_free(CPointerTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 result = CALL(pointer)->free(self->cvm, address);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CPointer_enter(CPointerTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CPointer_exit(CPointerTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CPointer_methods[] = {
    { "alloc", (PyCFunction)CPointer_alloc, METH_VARARGS, "Allocate a pointer" },
    { "copy", (PyCFunction)CPointer_copy, METH_VARARGS, "Copy a pointer" },
    { "read", (PyCFunction)CPointer_read, METH_VARARGS, "Read data from a pointer" },
    { "ref", (PyCFunction)CPointer_ref, METH_VARARGS, "Get reference count of a pointer" },
    { "free", (PyCFunction)CPointer_free, METH_VARARGS, "Free a pointer" },
    { "__enter__", (PyCFunction)CPointer_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CPointer_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CPointerTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CPointer",
    .tp_doc = "CPointer implementation in C module",
    .tp_basicsize = sizeof(CPointerType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CPointer_new,
    .tp_init = (initproc)CPointer_init,
    .tp_dealloc = (destructor)CPointer_dealloc,
    .tp_methods = CPointer_methods,
};

int init_cpointer(PyObject* module) {
    if (PyType_Ready(&CPointerTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CPointerTypeObject);
    if (PyModule_AddObject(module, "CPointer", (PyObject*)&CPointerTypeObject) < 0) {
        Py_DECREF(&CPointerTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
