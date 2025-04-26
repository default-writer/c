/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 25, 2025 at 7:06:03 AM GMT+3
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
#include "cexception.h"
#include "cvm.h"

#include "py_api.h"

/* static methods */
static PyObject* CEnvironment_getenv_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CEnvironment_getcwd_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CEnvironment_puts_static(PyObject* cls, PyObject* args, PyObject* kwargs);

static PyObject* CEnvironment_getenv_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!K|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(env)->getenv(cvm_py->cvm, src);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to retrieve environment variable: invalid pointer or variable not found: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CEnvironment_getcwd_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(env)->getcwd(cvm_py->cvm);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to get current working directory: invalid pointer or operation failed: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CEnvironment_puts_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!K|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(env)->puts(cvm_py->cvm, src);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to print string: invalid pointer or operation failed: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyMethodDef CEnvironment_methods[] = {
    /* CEnvironment static methods */
    { "getenv", (PyCFunction)CEnvironment_getenv_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Retrieve an environment variable" },
    { "getcwd", (PyCFunction)CEnvironment_getcwd_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Get the current working directory" },
    { "puts", (PyCFunction)CEnvironment_puts_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Print a string to the standard output" },
    { NULL, NULL, 0, NULL } /* Sentinel */
};

PyTypeObject CEnvironmentTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CEnvironment",
    .tp_doc = "CEnvironment implementation in C module",
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_methods = CEnvironment_methods,
};

int init_cenvironment(PyObject* module) {
    if (PyType_Ready(&CEnvironmentTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CEnvironmentTypeObject);
    if (PyModule_AddObject(module, "CEnvironment", (PyObject*)&CEnvironmentTypeObject) < 0) {
        Py_DECREF(&CEnvironmentTypeObject);
        return -1;
    }

    return 0;
}
