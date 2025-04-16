/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 16, 2025 at 11:03:49 AM GMT+3
 * Modified:
 *   April 16, 2025 at 6:39:06 PM GMT+3
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

#include "cuser.h"
#include "cvm.h"
#include "cexception.h"

static PyObject* CUser_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CUserTypePtr self;
    self = (CUserTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
        self->user_ptr = 0;
    }
    return (PyObject*)self;
}

static int CUser_init(CUserTypePtr self, PyObject* args, PyObject* kwds) {
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

    self->user_ptr = CALL(user)->alloc(self->cvm);
    if (self->user_ptr == 0) {
        PyErr_SetString(CInvalidPointerException, "Failed to allocate user object");
        return -1;
    }

    return 0;
}

static void CUser_dealloc(CUserTypePtr self) {
    if (self->user_ptr != 0) {
        CALL(user)->free(self->cvm, self->user_ptr);
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CUser_free(CUserTypePtr self, PyObject* Py_UNUSED(ignored)) {
    if (self->user_ptr != 0) {
        if (!CALL(user)->free(self->cvm, self->user_ptr)) {
            PyErr_SetString(CInvalidPointerException, CALL(error)->get());
            return NULL;
        }
        self->user_ptr = 0;
    }
    Py_RETURN_NONE;
}

static PyObject* CUser_enter(CUserTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CUser_exit(CUserTypePtr self, PyObject* args) {
    if (self->user_ptr != 0) {
        CALL(user)->free(self->cvm, self->user_ptr);
    }
    Py_RETURN_NONE;
}

static PyMethodDef CUser_methods[] = {
    { "free", (PyCFunction)CUser_free, METH_NOARGS, "Free the user object" },
    { "__enter__", (PyCFunction)CUser_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CUser_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CUserTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CUser",
    .tp_doc = "CUser implementation in C module",
    .tp_basicsize = sizeof(CUserType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CUser_new,
    .tp_init = (initproc)CUser_init,
    .tp_dealloc = (destructor)CUser_dealloc,
    .tp_methods = CUser_methods,
};

int init_cuser(PyObject* module) {
    if (PyType_Ready(&CUserTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CUserTypeObject);
    if (PyModule_AddObject(module, "CUser", (PyObject*)&CUserTypeObject) < 0) {
        Py_DECREF(&CUserTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
