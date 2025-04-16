/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 16, 2025 at 11:03:49 AM GMT+3
 * Modified:
 *   April 16, 2025 at 6:38:44 PM GMT+3
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

#include "cstack.h"
#include "cvm.h"
#include "cexception.h"

static PyObject* CStack_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CStackTypePtr self;
    self = (CStackTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
        self->stack_ptr = 0;
    }
    return (PyObject*)self;
}

static int CStack_init(CStackTypePtr self, PyObject* args, PyObject* kwds) {
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

    self->stack_ptr = CALL(stack)->alloc(self->cvm);
    if (self->stack_ptr == 0) {
        PyErr_SetString(CInvalidPointerException, "Failed to initialize the stack");
        return -1;
    }

    return 0;
}

static void CStack_dealloc(CStackTypePtr self) {
    if (self->stack_ptr != 0) {
        CALL(stack)->free(self->cvm, self->stack_ptr);
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CStack_push(CStackTypePtr self, PyObject* args) {
    u64 data_ptr;
    if (!PyArg_ParseTuple(args, "K", &data_ptr)) {
        return NULL;
    }

    if (!CALL(stack)->push(self->cvm, self->stack_ptr, data_ptr)) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CStack_pop(CStackTypePtr self, PyObject* Py_UNUSED(ignored)) {
    u64 data_ptr = CALL(stack)->pop(self->cvm, self->stack_ptr);
    if (data_ptr == 0) {
        PyErr_SetString(CInvalidValueException, "No elements to pop from the stack");
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(data_ptr);
}

static PyObject* CStack_peek(CStackTypePtr self, PyObject* Py_UNUSED(ignored)) {
    u64 data_ptr = CALL(stack)->peek(self->cvm, self->stack_ptr);
    if (data_ptr == 0) {
        PyErr_SetString(CInvalidValueException, "No elements to peek in the stack");
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(data_ptr);
}

static PyObject* CStack_size(CStackTypePtr self, PyObject* Py_UNUSED(ignored)) {
    u64 size = CALL(stack)->size(self->cvm, self->stack_ptr);
    return PyLong_FromUnsignedLongLong(size);
}

static PyObject* CStack_enter(CStackTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CStack_exit(CStackTypePtr self, PyObject* args) {
    if (self->stack_ptr != 0) {
        CALL(stack)->free(self->cvm, self->stack_ptr);
    }
    Py_RETURN_NONE;
}

static PyMethodDef CStack_methods[] = {
    { "push", (PyCFunction)CStack_push, METH_VARARGS, "Push an element onto the stack" },
    { "pop", (PyCFunction)CStack_pop, METH_NOARGS, "Pop an element from the stack" },
    { "peek", (PyCFunction)CStack_peek, METH_NOARGS, "Peek at the top element of the stack" },
    { "size", (PyCFunction)CStack_size, METH_NOARGS, "Get the size of the stack" },
    { "__enter__", (PyCFunction)CStack_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CStack_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CStackTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CStack",
    .tp_doc = "CStack implementation in C module",
    .tp_basicsize = sizeof(CStackType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CStack_new,
    .tp_init = (initproc)CStack_init,
    .tp_dealloc = (destructor)CStack_dealloc,
    .tp_methods = CStack_methods,
};

int init_cstack(PyObject* module) {
    if (PyType_Ready(&CStackTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CStackTypeObject);
    if (PyModule_AddObject(module, "CStack", (PyObject*)&CStackTypeObject) < 0) {
        Py_DECREF(&CStackTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
