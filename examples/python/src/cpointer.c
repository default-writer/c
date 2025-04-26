/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 26, 2025 at 11:28:32 AM GMT+3
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
#include "cexception.h"
#include "cvm.h"

#include "py_api.h"

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
    void_ptr data;
    u64 size, offset, type_id;
    if (!PyArg_ParseTuple(args, "OKKK|K", &cvm_obj, &size, &offset, &type_id, &data)) {
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

    u64 address = 0;
    if (data == NULL) {
        address = PY_CALL(pointer)->alloc(self->cvm, data, size, offset, FLAG_MEMORY_PTR, type_id);
    } else {
        address = PY_CALL(pointer)->copy(self->cvm, (const_void_ptr)data, size, offset, type_id);
    }
    if (!address) {
        PYTHON_ERROR(CInvalidArgumentException, "failed to allocatr/copy pointer: invalid arguments or source pointer: %s", CALL(error)->get());
        return -1;
    }
    self->ptr = address;

    return 0;
}

static void CPointer_dealloc(CPointerTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CPointer_read(CPointerTypePtr self, PyObject* args) {
    u64 address, type_id;
    if (!PyArg_ParseTuple(args, "KK", &address, &type_id)) {
        return NULL;
    }

    const_void_ptr data = PY_CALL(pointer)->read(self->cvm, address, type_id);
    if (data == NULL) {
        PYTHON_ERROR(CInvalidPointerException, "failed to read pointer: invalid address or type ID: %s", CALL(error)->get());
        return NULL;
    }

    return PyLong_FromVoidPtr((void_ptr)data);
}

static PyObject* CPointer_ref_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
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

    u64 result = PY_CALL(pointer)->ref(cvm_py->cvm, address);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to get reference count: invalid pointer address:  (%016llx) %s", address, CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CPointer_free_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
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

    u64 result = PY_CALL(pointer)->free(cvm_py->cvm, address);
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

static PyObject* CPointer_enter(CPointerTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CPointer_exit(CPointerTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CPointer_methods[] = {
    /* CPointer instance methods */
    { "read", (PyCFunction)CPointer_read, METH_VARARGS, "Read data from pointer" },

    /* CPointer static methods */
    { "ref", (PyCFunction)CPointer_ref_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Get reference pointer (static method)." },
    { "free", (PyCFunction)CPointer_free_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Free pointer (static method)." },

    /* CPointer context */
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
        return -1;
    }

    return 0;
}
