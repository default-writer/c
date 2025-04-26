/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 26, 2025 at 11:29:06 AM GMT+3
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
#include "cexception.h"
#include "cvm.h"

#include "py_api.h"

static PyObject* CStack_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

static int CStack_init(CStackTypePtr self, PyObject* args, PyObject* kwds);
static void CStack_dealloc(CStackTypePtr self);

/* instance methods */
static PyObject* CStack_ptr(CStackTypePtr self, PyObject* Py_UNUSED(ignored));
static PyObject* CStack_push(CStackTypePtr self, PyObject* args);
static PyObject* CStack_pop(CStackTypePtr self, PyObject* Py_UNUSED(ignored));
static PyObject* CStack_peek(CStackTypePtr self, PyObject* Py_UNUSED(ignored));
static PyObject* CStack_peekn(CStackTypePtr self, PyObject* args);
static PyObject* CStack_popn(CStackTypePtr self, PyObject* args);
static PyObject* CStack_size(CStackTypePtr self, PyObject* Py_UNUSED(ignored));
static PyObject* CStack_enter(CStackTypePtr self, PyObject* Py_UNUSED(ignored));
static PyObject* CStack_exit(CStackTypePtr self, PyObject* args);

/* static methods */
static PyObject* CStack_free_static(PyObject* cls, PyObject* args, PyObject* kwargs);

static PyObject* CStack_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CStackTypePtr self;
    self = (CStackTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
        self->ptr = 0;
    }
    return (PyObject*)self;
}

static int CStack_init(CStackTypePtr self, PyObject* args, PyObject* kwds) {
    PyObject* cvm_obj;
    u64 ptr = 0;
    if (!PyArg_ParseTuple(args, "O|K", &cvm_obj, &ptr)) {
        return -1;
    }

    if (!PyObject_TypeCheck(cvm_obj, &CVirtualMachineTypeObject)) {
        PYTHON_ERROR(PyExc_TypeError, "expected a CVirtualMachine instance: %s", CALL(error)->get());
        return -1;
    }

    CVirtualMachineTypePtr cvm = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer: %s", CALL(error)->get());
        return -1;
    }
    self->cvm = cvm->cvm;

    self->ptr = PY_CALL(stack)->alloc(self->cvm);
    if (self->ptr == 0) {
        PYTHON_ERROR(CInvalidPointerException, "failed to initialize the stack: %s", CALL(error)->get());
        return -1;
    }

    if (ptr != 0) {
        while (PY_CALL(stack)->size(self->cvm, ptr) > 0) {
            PY_CALL(stack)->push(self->cvm, self->ptr, PY_CALL(stack)->pop(self->cvm, ptr));
        }
    }

    return 0;
}

static void CStack_dealloc(CStackTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CStack_ptr(CStackTypePtr self, PyObject* Py_UNUSED(ignored)) {
    return PyLong_FromUnsignedLongLong(self->ptr);
}

static PyObject* CStack_push(CStackTypePtr self, PyObject* args) {
    u64 data_ptr;
    if (!PyArg_ParseTuple(args, "K", &data_ptr)) {
        return NULL;
    }

    if (!PY_CALL(stack)->push(self->cvm, self->ptr, data_ptr)) {
        PYTHON_ERROR(CInvalidArgumentException, "failed to push element onto the stack: invalid data or operation failed: %s", CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CStack_pop(CStackTypePtr self, PyObject* Py_UNUSED(ignored)) {
    u64 data_ptr = PY_CALL(stack)->pop(self->cvm, self->ptr);
    if (data_ptr == 0) {
        PYTHON_ERROR(CInvalidValueException, "no elements to pop from the stack: %s", CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(data_ptr);
}

static PyObject* CStack_peek(CStackTypePtr self, PyObject* Py_UNUSED(ignored)) {
    u64 data_ptr = PY_CALL(stack)->peek(self->cvm, self->ptr);
    if (data_ptr == 0) {
        PYTHON_ERROR(CInvalidValueException, "no elements to peek in the stack: %s", CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(data_ptr);
}

static PyObject* CStack_peekn(CStackTypePtr self, PyObject* args) {
    u64 nelements;
    if (!PyArg_ParseTuple(args, "K", &nelements)) {
        return NULL;
    }

    u64 result = PY_CALL(stack)->peekn(self->cvm, self->ptr, nelements);
    if (result == 0) {
        PYTHON_ERROR(CInvalidValueException, "failed to peek multiple elements from the stack: %s", CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CStack_popn(CStackTypePtr self, PyObject* args) {
    u64 nelements;
    if (!PyArg_ParseTuple(args, "K", &nelements)) {
        return NULL;
    }

    u64 result = PY_CALL(stack)->popn(self->cvm, self->ptr, nelements);
    if (result == 0) {
        PYTHON_ERROR(CInvalidValueException, "failed to pop multiple elements from the stack: %s", CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CStack_size(CStackTypePtr self, PyObject* Py_UNUSED(ignored)) {
    u64 size = PY_CALL(stack)->size(self->cvm, self->ptr);
    return PyLong_FromUnsignedLongLong(size);
}

static PyObject* CStack_free_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
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
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(stack)->free(cvm_py->cvm, address);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to free pointer: invalid pointer address: (%016llx) %s", address, CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CStack_enter(CStackTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CStack_exit(CStackTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CStack_methods[] = {
    /* CString instance methods */
    { "ptr", (PyCFunction)CStack_ptr, METH_NOARGS, "Returns the CStack's internal pointer address" },
    { "push", (PyCFunction)CStack_push, METH_VARARGS, "Push an element onto the stack" },
    { "peek", (PyCFunction)CStack_peek, METH_NOARGS, "Peek at the top element of the stack" },
    { "peekn", (PyCFunction)CStack_peekn, METH_VARARGS, "Peek multiple elements from the stack" },
    { "pop", (PyCFunction)CStack_pop, METH_NOARGS, "Pop an element from the stack" },
    { "popn", (PyCFunction)CStack_popn, METH_VARARGS, "Pop multiple elements from the stack" },
    { "size", (PyCFunction)CStack_size, METH_NOARGS, "Get the size of the stack" },

    /* CStack static methods */
    { "free", (PyCFunction)CStack_free_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Free pointer (static method)." },

    /* CStack context */
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
        return -1;
    }

    return 0;
}
