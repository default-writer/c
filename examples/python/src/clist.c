/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 16, 2025 at 11:03:49 AM GMT+3
 * Modified:
 *   April 16, 2025 at 6:37:36 PM GMT+3
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

#include "clist.h"
#include "cvm.h"
#include "cexception.h"

static PyObject* CList_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CListTypePtr self;
    self = (CListTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
        self->stack = NULL;
    }
    return (PyObject*)self;
}

static int CList_init(CListTypePtr self, PyObject* args, PyObject* kwds) {
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

    self->stack = CALL(list)->init(self->cvm);
    if (self->stack == NULL) {
        PyErr_SetString(CInvalidPointerException, "Failed to initialize the list");
        return -1;
    }

    return 0;
}

static void CList_dealloc(CListTypePtr self) {
    if (self->stack != NULL) {
        CALL(list)->destroy(self->cvm, self->stack);
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CList_push(CListTypePtr self, PyObject* args) {
    void_ptr data;
    if (!PyArg_ParseTuple(args, "k", &data)) {
        return NULL;
    }

    if (!CALL(list)->push(self->cvm, self->stack, data)) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CList_pop(CListTypePtr self, PyObject* Py_UNUSED(ignored)) {
    void_ptr data = CALL(list)->pop(self->cvm, self->stack);
    if (data == NULL) {
        PyErr_SetString(CInvalidValueException, "No elements to pop from the list");
        return NULL;
    }

    return PyLong_FromVoidPtr(data);
}

static PyObject* CList_peek(CListTypePtr self, PyObject* Py_UNUSED(ignored)) {
    void_ptr data = CALL(list)->peek(self->cvm, self->stack);
    if (data == NULL) {
        PyErr_SetString(CInvalidValueException, "No elements to peek in the list");
        return NULL;
    }

    return PyLong_FromVoidPtr(data);
}

static PyObject* CList_enter(CListTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CList_exit(CListTypePtr self, PyObject* args) {
    if (self->stack != NULL) {
        CALL(list)->destroy(self->cvm, self->stack);
    }
    Py_RETURN_NONE;
}

static PyMethodDef CList_methods[] = {
    { "push", (PyCFunction)CList_push, METH_VARARGS, "Push an element onto the list" },
    { "pop", (PyCFunction)CList_pop, METH_NOARGS, "Pop an element from the list" },
    { "peek", (PyCFunction)CList_peek, METH_NOARGS, "Peek at the top element of the list" },
    { "__enter__", (PyCFunction)CList_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CList_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CListTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CList",
    .tp_doc = "CList implementation in C module",
    .tp_basicsize = sizeof(CListType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CList_new,
    .tp_init = (initproc)CList_init,
    .tp_dealloc = (destructor)CList_dealloc,
    .tp_methods = CList_methods,
};

int init_clist(PyObject* module) {
    if (PyType_Ready(&CListTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CListTypeObject);
    if (PyModule_AddObject(module, "CList", (PyObject*)&CListTypeObject) < 0) {
        Py_DECREF(&CListTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
