/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 26, 2025 at 11:27:48 AM GMT+3
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

#include "cfile.h"
#include "cexception.h"
#include "cvm.h"

#include "py_api.h"

static PyObject* CFile_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CFileTypePtr self;
    self = (CFileTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
        self->ptr = 0;
    }
    return (PyObject*)self;
}

static int CFile_init(CFileTypePtr self, PyObject* args, PyObject* kwds) {
    PyObject* cvm_obj;
    u64 file_path_ptr, mode_ptr;
    if (!PyArg_ParseTuple(args, "OKK", &cvm_obj, &file_path_ptr, &mode_ptr)) {
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

    u64 ptr = PY_CALL(file)->alloc(self->cvm, file_path_ptr, mode_ptr);
    if (!ptr) {
        PYTHON_ERROR(CInvalidArgumentException, "failed to allocate file: invalid file path or mode: %s", CALL(error)->get());
        return -1;
    }
    self->ptr = ptr;

    return 0;
}

static void CFile_dealloc(CFileTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CFile_data(CFileTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 data_address = PY_CALL(file)->data(self->cvm, address);
    if (!data_address) {
        PYTHON_ERROR(CInvalidPointerException, "failed to retrieve file data: invalid file address: %s", CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(data_address);
}

static PyObject* CFile_free_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
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

    u64 result = PY_CALL(file)->free(cvm_py->cvm, address);
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

static PyObject* CFile_enter(CFileTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CFile_exit(CFileTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CFile_methods[] = {
    { "data", (PyCFunction)CFile_data, METH_VARARGS, "Get file data" },

    /* CFile static methods */
    { "free", (PyCFunction)CFile_free_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Free pointer (static method)." },

    /* CFile context */
    { "__enter__", (PyCFunction)CFile_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CFile_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CFileTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CFile",
    .tp_doc = "CFile implementation in C module",
    .tp_basicsize = sizeof(CFileType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CFile_new,
    .tp_init = (initproc)CFile_init,
    .tp_dealloc = (destructor)CFile_dealloc,
    .tp_methods = CFile_methods,
};

int init_cfile(PyObject* module) {
    if (PyType_Ready(&CFileTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CFileTypeObject);
    if (PyModule_AddObject(module, "CFile", (PyObject*)&CFileTypeObject) < 0) {
        Py_DECREF(&CFileTypeObject);
        return -1;
    }

    return 0;
}
