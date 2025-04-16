/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 16, 2025 at 11:03:49 AM GMT+3
 * Modified:
 *   April 16, 2025 at 6:31:51 PM GMT+3
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

#include "cenvironment.h"
#include "cvm.h"
#include "cexception.h"

static PyObject* CEnvironment_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CEnvironmentTypePtr self;
    self = (CEnvironmentTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
    }
    return (PyObject*)self;
}

static int CEnvironment_init(CEnvironmentTypePtr self, PyObject* args, PyObject* kwds) {
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

static void CEnvironment_dealloc(CEnvironmentTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CEnvironment_getenv(CEnvironmentTypePtr self, PyObject* args) {
    u64 name_ptr;
    if (!PyArg_ParseTuple(args, "K", &name_ptr)) {
        return NULL;
    }

    u64 result = CALL(env)->getenv(self->cvm, name_ptr);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CEnvironment_getcwd(CEnvironmentTypePtr self, PyObject* Py_UNUSED(ignored)) {
    u64 result = CALL(env)->getcwd(self->cvm);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CEnvironment_puts(CEnvironmentTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 result = CALL(env)->puts(self->cvm, address);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CEnvironment_enter(CEnvironmentTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CEnvironment_exit(CEnvironmentTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CEnvironment_methods[] = {
    { "getenv", (PyCFunction)CEnvironment_getenv, METH_VARARGS, "Retrieve an environment variable" },
    { "getcwd", (PyCFunction)CEnvironment_getcwd, METH_NOARGS, "Get the current working directory" },
    { "puts", (PyCFunction)CEnvironment_puts, METH_VARARGS, "Print a string to the standard output" },
    { "__enter__", (PyCFunction)CEnvironment_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CEnvironment_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CEnvironmentTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CEnvironment",
    .tp_doc = "CEnvironment implementation in C module",
    .tp_basicsize = sizeof(CEnvironmentType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CEnvironment_new,
    .tp_init = (initproc)CEnvironment_init,
    .tp_dealloc = (destructor)CEnvironment_dealloc,
    .tp_methods = CEnvironment_methods,
};

int init_cenvironment(PyObject* module) {
    if (PyType_Ready(&CEnvironmentTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CEnvironmentTypeObject);
    if (PyModule_AddObject(module, "CEnvironment", (PyObject*)&CEnvironmentTypeObject) < 0) {
        Py_DECREF(&CEnvironmentTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
