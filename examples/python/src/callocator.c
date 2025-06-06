/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 12, 2025 at 5:02:55 AM GMT+3
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

#include "callocator.h"
#include "cexception.h"
#include "cvm.h"

#include "py_api.h"

/* alloc */
static PyObject* CVirtual_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

/* constructor/destructor */
static int CVirtual_init(CAllocatorTypePtr self, PyObject* args, PyObject* kwds);
static void CVirtual_dealloc(CAllocatorTypePtr self);

/* instance methods */
static PyObject* CVirtual_alloc(CAllocatorTypePtr self, PyObject* args);
static PyObject* CVirtual_read(CAllocatorTypePtr self, PyObject* args);
static PyObject* CVirtual_type(CAllocatorTypePtr self, PyObject* args);

/* static methods */
static PyObject* CVirtual_free_static(PyObject* cls, PyObject* args, PyObject* kwargs);

/* context manager protocol */
static PyObject* CVirtual_enter(CAllocatorTypePtr self, PyObject* Py_UNUSED(ignored));
static PyObject* CVirtual_exit(CAllocatorTypePtr self, PyObject* args);

static PyObject* CVirtual_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CAllocatorTypePtr self;
    self = (CAllocatorTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
    }
    return (PyObject*)self;
}

static int CVirtual_init(CAllocatorTypePtr self, PyObject* args, PyObject* kwds) {
    PyObject* cvm_obj;
    if (!PyArg_ParseTuple(args, "O", &cvm_obj)) {
        return -1;
    }

    if (!PyObject_TypeCheck(cvm_obj, &CVirtualMachineTypeObject)) {
        PYTHON_ERROR(PyExc_TypeError, "expected a CVirtualMachine instance");
        return -1;
    }

    CVirtualMachineTypePtr cvm = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer");
        return -1;
    }
    self->cvm = cvm->cvm;

    return 0;
}

static void CVirtual_dealloc(CAllocatorTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CVirtual_alloc(CAllocatorTypePtr self, PyObject* args) {
    u64 size, type_id;
    if (!PyArg_ParseTuple(args, "KK", &size, &type_id)) {
        return NULL;
    }

    u64 address = PY_CALL(allocator)->alloc(self->cvm, size, type_id);
    if (!address) {
        PYTHON_ERROR(CInvalidArgumentException, "failed to allocate memory: invalid size or type ID");
        return NULL;
    }

    self->ptr = address;

    return PyLong_FromUnsignedLongLong(address);
}

static PyObject* CVirtual_read(CAllocatorTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    const_pointer_ptr data = PY_CALL(allocator)->read(self->cvm, address);
    if (data == NULL) {
        PYTHON_ERROR(CInvalidPointerException, "failed to read memory: invalid address");
        return NULL;
    }

    return PyLong_FromVoidPtr((void_ptr)data);
}

static PyObject* CVirtual_type(CAllocatorTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 type_id = PY_CALL(allocator)->type(self->cvm, address);
    if (!type_id) {
        PYTHON_ERROR(CInvalidTypeIdException, "failed to get type ID: invalid address");
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(type_id);
}

static PyObject* CVirtual_free_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
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
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance");
        return NULL;
    }

    u64 result = PY_CALL(allocator)->free(cvm_py->cvm, address);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to free pointer: invalid pointer address: %d", address);
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CVirtual_enter(CAllocatorTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CVirtual_exit(CAllocatorTypePtr self, PyObject* args) {
    PyObject *exc_type, *exc_value, *traceback;
    if (!PyArg_ParseTuple(args, "OOO", &exc_type, &exc_value, &traceback)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyMethodDef CAllocator_methods[] = {
    /* CAllocator instance methods*/
    { "alloc", (PyCFunction)CVirtual_alloc, METH_VARARGS, "Allocate memory in the virtual machine" },
    { "read", (PyCFunction)CVirtual_read, METH_VARARGS, "Read memory from the virtual machine" },
    { "type", (PyCFunction)CVirtual_type, METH_VARARGS, "Get the type of a memory address" },

    /* CAllocator static methods */
    { "free", (PyCFunction)CVirtual_free_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Free pointer (static method)." },

    /* CAllocator context */
    { "__enter__", (PyCFunction)CVirtual_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CVirtual_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CAllocatorTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CAllocator",
    .tp_doc = "CAllocator implementation in C module",
    .tp_basicsize = sizeof(CAllocatorType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CVirtual_new,
    .tp_init = (initproc)CVirtual_init,
    .tp_dealloc = (destructor)CVirtual_dealloc,
    .tp_methods = CAllocator_methods,
};

int init_cvirtual(PyObject* module) {
    if (PyType_Ready(&CAllocatorTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CAllocatorTypeObject);
    if (PyModule_AddObject(module, "CAllocator", (PyObject*)&CAllocatorTypeObject) < 0) {
        Py_DECREF(&CAllocatorTypeObject);
        return -1;
    }

    return 0;
}
