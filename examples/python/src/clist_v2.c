/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   May 4, 2025 at 5:08:58 AM GMT+3
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

#include "clist_v2.h"
#include "cexception.h"

#include "py_api.h"

/* alloc */
static PyObject* CListV2_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

/* constructor/destructor */
static int CListV2_init(CListV2TypePtr self, PyObject* args);
static void CListV2_dealloc(CListV2TypePtr self);

/* instance methods */
static PyObject* CListV2_push(CListV2TypePtr self, PyObject* args);
static PyObject* CListV2_pop(CListV2TypePtr self, PyObject* Py_UNUSED(ignored));
static PyObject* CListV2_peek(CListV2TypePtr self, PyObject* Py_UNUSED(ignored));
static PyObject* CListV2_size(CListV2TypePtr self, PyObject* Py_UNUSED(ignored));
static PyObject* CListV2_diff(CListV2TypePtr self, PyObject* args);
static PyObject* CListV2_diff_left(CListV2TypePtr self, PyObject* args);
static PyObject* CListV2_diff_right(CListV2TypePtr self, PyObject* args);

/* static methods */

/* context manager protocol */
static PyObject* CListV2_enter(CListV2TypePtr self, PyObject* Py_UNUSED(ignored));
static PyObject* CListV2_exit(CListV2TypePtr self, PyObject* args);

static PyObject* CListV2_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CListV2TypePtr self;
    self = (CListV2TypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->stack = NULL;
    }
    return (PyObject*)self;
}

static int CListV2_init(CListV2TypePtr self, PyObject* args) {
    u64 size;
    if (!PyArg_ParseTuple(args, "K", &size)) {
        return -1;
    }

    self->stack = PY_CALL(list_v2)->init(size, NULL_PTR);
    if (self->stack == NULL) {
        PYTHON_ERROR(CInvalidPointerException, "failed to initialize the list: %s", CALL(error)->get());
        return -1;
    }

    return 0;
}

static void CListV2_dealloc(CListV2TypePtr self) {
    if (self->stack != NULL) {
        PY_CALL(list_v2)->destroy(&self->stack);
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CListV2_push(CListV2TypePtr self, PyObject* args) {
    void_ptr data;
    if (!PyArg_ParseTuple(args, "K", &data)) {
        return NULL;
    }

    if (!PY_CALL(list_v2)->push(&self->stack, data)) {
        u64 error_type = CALL(error)->type();
        if (error_type == ID_ERROR_VM_NOT_INITIALIZED) {
            PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer: %s", CALL(error)->get());
        } else if (error_type == ID_ERROR_INVALID_ARGUMENT) {
            PYTHON_ERROR(CInvalidArgumentException, "invalid argument during push: %s", CALL(error)->get());
        } else {
            PYTHON_ERROR(CException, "failed to push element onto the list: %s", CALL(error)->get());
        }
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CListV2_pop(CListV2TypePtr self, PyObject* Py_UNUSED(ignored)) {
    void_ptr data = PY_CALL(list_v2)->pop(&self->stack);
    u64 error_type = CALL(error)->type();
    if (error_type == ID_ERROR_VM_NOT_INITIALIZED) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "vm not initialized during pop: %s", CALL(error)->get());
        return NULL;
    }
    if (error_type == ID_ERROR_INVALID_ARGUMENT) {
        PYTHON_ERROR(CInvalidArgumentException, "invalid argument during pop: %s", CALL(error)->get());
        return NULL;
    }
    return PyLong_FromVoidPtr(data);
}

static PyObject* CListV2_peek(CListV2TypePtr self, PyObject* Py_UNUSED(ignored)) {
    void_ptr data = PY_CALL(list_v2)->peek(&self->stack);
    u64 error_type = CALL(error)->type();
    if (error_type == ID_ERROR_VM_NOT_INITIALIZED) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "vm not initialized during peek: %s", CALL(error)->get());
        return NULL;
    }
    if (error_type == ID_ERROR_INVALID_ARGUMENT) {
        PYTHON_ERROR(CInvalidArgumentException, "invalid argument during peek: %s", CALL(error)->get());
        return NULL;
    }
    return PyLong_FromVoidPtr(data);
}

static PyObject* CListV2_size(CListV2TypePtr self, PyObject* Py_UNUSED(ignored)) {
    // Check for potential errors before accessing size, although less likely here
    u64 error_type = CALL(error)->type();
    if (error_type == ID_ERROR_VM_NOT_INITIALIZED) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "vm not initialized before getting size: %s", CALL(error)->get());
        return NULL;
    }
    if (self->stack == NULL) {
        PYTHON_ERROR(CInvalidPointerException, "list not initialized before getting size: %s", CALL(error)->get());
        return NULL;
    }
    u64 size = self->stack->size;
    error_type = CALL(error)->type();
    if (error_type == ID_ERROR_INVALID_ARGUMENT) { // Check if accessing size itself caused an error
        PYTHON_ERROR(CInvalidArgumentException, "invalid argument during size: %s", CALL(error)->get());
        return NULL;
    }
    return PyLong_FromUnsignedLong(size);
}

static PyObject* CListV2_diff(CListV2TypePtr self, PyObject* args) {
    PyObject *other_list_obj, *result_list_obj;
    CListV2TypePtr other_list, result_list;

    if (!PyArg_ParseTuple(args, "O!O!", &CListV2TypeObject, &other_list_obj, &CListV2TypeObject, &result_list_obj)) {
        return NULL;
    }

    other_list = (CListV2TypePtr)other_list_obj;
    result_list = (CListV2TypePtr)result_list_obj;

    if (self->stack == NULL || other_list->stack == NULL || result_list->stack == NULL) {
        PYTHON_ERROR(CInvalidPointerException, "one or more lists involved in diff are not initialized: %s", CALL(error)->get());
        return NULL;
    }

    if (!PY_CALL(list_v2)->diff(self->stack, other_list->stack, &result_list->stack)) {
        u64 error_type = CALL(error)->type();
        if (error_type == ID_ERROR_VM_NOT_INITIALIZED) {
            PYTHON_ERROR(CVirtualMachineNotInitializedException, "vm not initialized during diff: %s", CALL(error)->get());
        } else if (error_type == ID_ERROR_INVALID_ARGUMENT) {
            PYTHON_ERROR(CInvalidArgumentException, "invalid argument during diff: %s", CALL(error)->get());
        } else {
            PYTHON_ERROR(CException, "list diff operation failed: %s", CALL(error)->get());
        }
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CListV2_diff_left(CListV2TypePtr self, PyObject* args) {
    PyObject *other_list_obj, *result_list_obj;
    CListV2TypePtr other_list, result_list;

    if (!PyArg_ParseTuple(args, "O!O!", &CListV2TypeObject, &other_list_obj, &CListV2TypeObject, &result_list_obj)) {
        return NULL;
    }

    other_list = (CListV2TypePtr)other_list_obj;
    result_list = (CListV2TypePtr)result_list_obj;

    if (self->stack == NULL || other_list->stack == NULL || result_list->stack == NULL) {
        PYTHON_ERROR(CInvalidPointerException, "one or more lists involved in diff_left are not initialized: %s", CALL(error)->get());
        return NULL;
    }

    if (!PY_CALL(list_v2)->diff_left(self->stack, other_list->stack, &result_list->stack)) {
        u64 error_type = CALL(error)->type();
        if (error_type == ID_ERROR_VM_NOT_INITIALIZED) {
            PYTHON_ERROR(CVirtualMachineNotInitializedException, "vm not initialized during diff_left: %s", CALL(error)->get());
        } else if (error_type == ID_ERROR_INVALID_ARGUMENT) {
            PYTHON_ERROR(CInvalidArgumentException, "invalid argument during diff_left: %s", CALL(error)->get());
        } else {
            PYTHON_ERROR(CException, "list diff_left operation failed: %s", CALL(error)->get());
        }
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CListV2_diff_right(CListV2TypePtr self, PyObject* args) {
    PyObject *other_list_obj, *result_list_obj;
    CListV2TypePtr other_list, result_list;

    if (!PyArg_ParseTuple(args, "O!O!", &CListV2TypeObject, &other_list_obj, &CListV2TypeObject, &result_list_obj)) {
        return NULL;
    }

    other_list = (CListV2TypePtr)other_list_obj;
    result_list = (CListV2TypePtr)result_list_obj;

    if (self->stack == NULL || other_list->stack == NULL || result_list->stack == NULL) {
        PYTHON_ERROR(CInvalidPointerException, "one or more lists involved in diff_right are not initialized: %s", CALL(error)->get());
        return NULL;
    }

    if (!PY_CALL(list_v2)->diff_right(self->stack, other_list->stack, &result_list->stack)) {
        u64 error_type = CALL(error)->type();
        if (error_type == ID_ERROR_VM_NOT_INITIALIZED) {
            PYTHON_ERROR(CVirtualMachineNotInitializedException, "vm not initialized during diff_right: %s", CALL(error)->get());
        } else if (error_type == ID_ERROR_INVALID_ARGUMENT) {
            PYTHON_ERROR(CInvalidArgumentException, "invalid argument during diff_right: %s", CALL(error)->get());
        } else {
            PYTHON_ERROR(CException, "list diff_right operation failed: %s", CALL(error)->get());
        }
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CListV2_enter(CListV2TypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CListV2_exit(CListV2TypePtr self, PyObject* args) {
    PyObject *exc_type, *exc_value, *traceback;
    if (!PyArg_ParseTuple(args, "OOO", &exc_type, &exc_value, &traceback)) {
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyMethodDef CListV2_methods[] = {
    { "push", (PyCFunction)CListV2_push, METH_VARARGS, "push(value: int) -> None\n\nPush an integer value (representing a pointer) onto the list." },
    { "pop", (PyCFunction)CListV2_pop, METH_NOARGS, "pop() -> int\n\nPop an integer value (representing a pointer) from the list.\nRaises CInvalidArgumentException if the list is empty or on other C-level errors." },
    { "size", (PyCFunction)CListV2_size, METH_NOARGS, "size() -> int\n\nReturn the number of elements in the list." },
    { "peek", (PyCFunction)CListV2_peek, METH_NOARGS, "peek() -> int\n\nReturn the top integer value (representing a pointer) without removing it.\nRaises CInvalidArgumentException if the list is empty or on other C-level errors." },
    { "diff", (PyCFunction)CListV2_diff, METH_VARARGS, "diff(other: CListV2, result: CListV2) -> None\n\nCompute the symmetric difference (elements in self or other, but not both)\nand store the result in the 'result' CListV2. All lists must be initialized." },
    { "diff_left", (PyCFunction)CListV2_diff_left, METH_VARARGS, "diff_left(other: CListV2, result: CListV2) -> None\n\nCompute the left difference (elements in self but not in other)\nand store the result in the 'result' CListV2. All lists must be initialized." },
    { "diff_right", (PyCFunction)CListV2_diff_right, METH_VARARGS, "diff_right(other: CListV2, result: CListV2) -> None\n\nCompute the right difference (elements in other but not in self)\nand store the result in the 'result' CListV2. All lists must be initialized." },

    /* CListV2 context manager support */
    { "__enter__", (PyCFunction)CListV2_enter, METH_NOARGS, "Enter the runtime context related to this object." },
    { "__exit__", (PyCFunction)CListV2_exit, METH_VARARGS, "Exit the runtime context. Resources are automatically managed." },
    { NULL } /* Sentinel */
};

PyTypeObject CListV2TypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CList2",
    .tp_doc = PyDoc_STR(
        "CListV2()\n\n"
        "A Python wrapper for an efficient C-level list implementation.\n\n"
        "This list stores integer values which internally represent C pointers (void*).\n"
        "It provides basic stack-like operations (push, pop, peek, size) and\n"
        "set difference operations (diff, diff_left, diff_right).\n\n"
        "The list acts as a container for opaque handles or pointers managed by\n"
        "other parts of the C library.\n\n"
        "It supports the context manager protocol ('with' statement), ensuring\n"
        "proper resource management via its deallocator.\n\n"
        "Methods like 'pop' and 'peek' will raise appropriate CExceptions\n"
        "(e.g., CInvalidArgumentException) if the list is empty or other errors\n"
        "occur in the underlying C implementation."),
    .tp_basicsize = sizeof(CListV2Type),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CListV2_new,
    .tp_init = (initproc)CListV2_init,
    .tp_dealloc = (destructor)CListV2_dealloc,
    .tp_methods = CListV2_methods,
};

int init_clist_v2(PyObject* module) {
    if (PyType_Ready(&CListV2TypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CListV2TypeObject);
    if (PyModule_AddObject(module, "CList2", (PyObject*)&CListV2TypeObject) < 0) {
        Py_DECREF(&CListV2TypeObject);
        return -1;
    }

    return 0;
}
