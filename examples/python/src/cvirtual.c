/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 16, 2025 at 11:03:49 AM GMT+3
 * Modified:
 *   April 16, 2025 at 6:39:15 PM GMT+3
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

#include "cvirtual.h"
#include "cvm.h"
#include "cexception.h"

static PyObject* CVirtual_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CVirtualTypePtr self;
    self = (CVirtualTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
    }
    return (PyObject*)self;
}

static int CVirtual_init(CVirtualTypePtr self, PyObject* args, PyObject* kwds) {
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

static void CVirtual_dealloc(CVirtualTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CVirtual_alloc(CVirtualTypePtr self, PyObject* args) {
    u64 size, type_id;
    if (!PyArg_ParseTuple(args, "KK", &size, &type_id)) {
        return NULL;
    }

    u64 address = CALL(virtual)->alloc(self->cvm, size, type_id);
    if (!address) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(address);
}

static PyObject* CVirtual_read(CVirtualTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    const_pointer_ptr data = CALL(virtual)->read(self->cvm, address);
    if (data == NULL) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromVoidPtr((void_ptr)data);
}

static PyObject* CVirtual_type(CVirtualTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 type_id = CALL(virtual)->type(self->cvm, address);
    if (!type_id) {
        PyErr_SetString(CInvalidTypeIdException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(type_id);
}

static PyObject* CVirtual_free(CVirtualTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 result = CALL(virtual)->free(self->cvm, address);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CVirtual_enter(CVirtualTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CVirtual_exit(CVirtualTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CVirtual_methods[] = {
    { "alloc", (PyCFunction)CVirtual_alloc, METH_VARARGS, "Allocate memory in the virtual machine" },
    { "read", (PyCFunction)CVirtual_read, METH_VARARGS, "Read memory from the virtual machine" },
    { "type", (PyCFunction)CVirtual_type, METH_VARARGS, "Get the type of a memory address" },
    { "free", (PyCFunction)CVirtual_free, METH_VARARGS, "Free memory in the virtual machine" },
    { "__enter__", (PyCFunction)CVirtual_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CVirtual_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CVirtualTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CVirtual",
    .tp_doc = "CVirtual implementation in C module",
    .tp_basicsize = sizeof(CVirtualType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CVirtual_new,
    .tp_init = (initproc)CVirtual_init,
    .tp_dealloc = (destructor)CVirtual_dealloc,
    .tp_methods = CVirtual_methods,
};

int init_cvirtual(PyObject* module) {
    if (PyType_Ready(&CVirtualTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CVirtualTypeObject);
    if (PyModule_AddObject(module, "CVirtual", (PyObject*)&CVirtualTypeObject) < 0) {
        Py_DECREF(&CVirtualTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
