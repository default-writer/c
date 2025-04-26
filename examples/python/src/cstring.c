/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 12, 1961 at 09:07:34 PM GMT+3
 * Modified:
 *   April 26, 2025 at 11:29:31 AM GMT+3
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

#include "cstring.h"
#include "cexception.h"
#include "clist.h"
#include "cvm.h"

#include "py_api.h"

static PyObject* CString_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

static int CString_init(CStringTypePtr self, PyObject* args, PyObject* kwds);
static void CString_dealloc(CStringTypePtr self);

/* instance methods */
static PyObject* CString_ptr(CStringTypePtr self, PyObject* Py_UNUSED(ignored));

/* static methods */
static PyObject* CString_free_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_split_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_size_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_copy_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_strcpy_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_strcat_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_strrchr_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_strchr_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_match_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_match_offset_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_put_char_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_unsafe_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_lessthan_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_greaterthan_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_equals_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_compare_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_left_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_right_copy_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_left_copy_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_right_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_move_left_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_move_right_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_strcmp_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_ref_static(PyObject* cls, PyObject* args, PyObject* kwargs);
static PyObject* CString_enter(CStringTypePtr self, PyObject* Py_UNUSED(ignored));
static PyObject* CString_exit(CStringTypePtr self, PyObject* args);

static const char* CString_Empty = "\0";

static PyObject* CString_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CStringTypePtr self;
    self = (CStringTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
        self->ptr = 0;
    }
    return (PyObject*)self;
}

static void CString_dealloc(CStringTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static int CString_init(CStringTypePtr self, PyObject* args, PyObject* kwds) {
    PyObject* cvm_obj;
    const char* data;
    u64 size = 0;
    if (!PyArg_ParseTuple(args, "Os|K", &cvm_obj, &data, &size)) {
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

    u64 ptr = PY_CALL(string)->load(self->cvm, data);
    if (!ptr) {
        PYTHON_ERROR(CInvalidArgumentException, "failed to load string: invalid data or operation failed: %s", CALL(error)->get());
        return -1;
    }
    self->ptr = ptr;

    return 0;
}

static PyObject* CString_ptr(CStringTypePtr self, PyObject* Py_UNUSED(ignored)) {
    return PyLong_FromUnsignedLongLong(self->ptr);
}

static PyObject* CString_free_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
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

    u64 result = PY_CALL(string)->free(cvm_py->cvm, address);
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

static PyObject* CString_split_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src;
    PyObject* cvm_obj = NULL;
    PyObject* list_obj;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "stack", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KO!|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src,
            &CListTypeObject, &list_obj,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    CListTypePtr clist = (CListTypePtr)list_obj;
    u64 result = PY_CALL(string)->split(cvm_py->cvm, src, clist->stack);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to size string: invalid source address: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_size_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
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

    u64 result = PY_CALL(string)->size(cvm_py->cvm, src);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to size string: invalid source address: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_copy_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
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

    u64 result = PY_CALL(string)->copy(cvm_py->cvm, src);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to copy string: invalid source address: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_strncpy_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 dest, src;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "dest", "src", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &dest, &src,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    PY_CALL(string)->strncpy(cvm_py->cvm, dest, src);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to strncpy string: invalid source or destination address: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
    }

    Py_RETURN_NONE;
}

static PyObject* CString_strcat_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 dest, src;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "dest", "src", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &dest, &src,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    PY_CALL(string)->strcat(cvm_py->cvm, dest, src);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to concatenate strings: invalid source or destination address: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
    }

    Py_RETURN_NONE;
}

static PyObject* CString_strrchr_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, match;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "match", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &match,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->strrchr(cvm_py->cvm, src, match);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to find last occurrence of character: invalid source or match address: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_strchr_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, match;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "match", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &match,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->strchr(cvm_py->cvm, src, match);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to find first occurrence of character: invalid source or match address: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_match_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, match;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "match", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &match,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->match(cvm_py->cvm, src, match);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to match string: invalid source or match address: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_match_offset_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, match;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "match", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &match,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->match_offset(cvm_py->cvm, src, match);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to calculate offset: invalid source or match address: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_put_char_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 ptr;
    char value;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "ptr", "value", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!Kb|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &ptr, &value,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    PY_CALL(string)->put_char(cvm_py->cvm, ptr, value);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to put character: invalid address or value: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
    }

    Py_RETURN_NONE;
}

static PyObject* CString_unsafe_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "nothrow", NULL };

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

    const char* data = PY_CALL(string)->unsafe(cvm_py->cvm, src);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to get raw string data: invalid address or operation failed: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        data = CString_Empty;
    }

    return PyUnicode_FromString(data);
}

static PyObject* CString_lessthan_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, dest;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "dest", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &dest,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->lessthan(cvm_py->cvm, src, dest);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to compare strings (lessthan): invalid source or destination address: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_greaterthan_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, dest;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "dest", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &dest,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->greaterthan(cvm_py->cvm, src, dest);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to compare strings (greaterthan): invalid source or destination address: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_equals_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, dest;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "dest", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &dest,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->equals(cvm_py->cvm, src, dest);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to check string equality: invalid source or destination address: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_compare_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, dest;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "dest", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &dest,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->compare(cvm_py->cvm, src, dest);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to compare strings: invalid source or destination address or C error occurred: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_left_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, offset;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "offset", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &offset,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->left(cvm_py->cvm, src, offset);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to get left part of string: invalid source or offset: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_right_copy_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, nbytes;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "nbytes", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &nbytes,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->right_copy(cvm_py->cvm, src, nbytes);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to copy n bytes of string: invalid source or number of bytes: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_left_copy_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, nbytes;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "nbytes", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &nbytes,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->left_copy(cvm_py->cvm, src, nbytes);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to copy n bytes from left of string: invalid source or number of bytes: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_right_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, offset;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "offset", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &offset,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->right(cvm_py->cvm, src, offset);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to get right part of string: invalid source or offset: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_move_left_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, nbytes;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "nbytes", "nothrow", NULL }; // Changed offset to nbytes

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &nbytes, // Changed offset to nbytes
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    PY_CALL(string)->move_left(cvm_py->cvm, src, nbytes);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to move string left: invalid source or number of bytes: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
    }

    Py_RETURN_NONE;
}

static PyObject* CString_move_right_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, nbytes;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "nbytes", "nothrow", NULL }; // Changed offset to nbytes

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &nbytes, // Changed offset to nbytes
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    PY_CALL(string)->move_right(cvm_py->cvm, src, nbytes);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to move string right: invalid source or number of bytes: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
    }

    Py_RETURN_NONE;
}

static PyObject* CString_strcmp_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, dest;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "dest", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &dest,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->strcmp(cvm_py->cvm, src, dest);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to compare strings (strcmp): invalid source or destination address or C error occurred: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_ref_static(PyObject* cls, PyObject* args, PyObject* kwargs) {
    u64 src, depth;
    PyObject* cvm_obj = NULL;
    PyObject* nothrow_obj = Py_False;
    static char* keywords[] = { "cvm", "src", "depth", "nothrow", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O!KK|$O!", keywords,
            &CVirtualMachineTypeObject, &cvm_obj,
            &src, &depth,
            &PyBool_Type, &nothrow_obj)) {
        return NULL;
    }

    CVirtualMachineTypePtr cvm_py = (CVirtualMachineTypePtr)cvm_obj;
    if (cvm_py->cvm == NULL) {
        PYTHON_ERROR(CVirtualMachineNotInitializedException, "invalid CVirtualMachine pointer in provided cvm instance: %s", CALL(error)->get());
        return NULL;
    }

    u64 result = PY_CALL(string)->ref(cvm_py->cvm, src, depth);
    u64 error_type = CALL(error)->type();
    if (error_type != 0) {
        int nothrow = PyObject_IsTrue(nothrow_obj);
        if (!nothrow) {
            PYTHON_ERROR(CInvalidPointerException, "failed to get referenced strings (depth): invalid source or destination address or C error occurred: %s", CALL(error)->get());
            return NULL;
        }
        CALL(error)->clear();
        result = 0;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CString_enter(CStringTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CString_exit(CStringTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CString_methods[] = {
    /* CString instance methods */
    { "ptr", (PyCFunction)CString_ptr, METH_NOARGS, "Returns the CString's internal pointer address" },

    /* CString static methods */
    { "free", (PyCFunction)CString_free_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Free pointer (static method)." },
    { "split", (PyCFunction)CString_split_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Split string into line chunks (static method)." },
    { "size", (PyCFunction)CString_size_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Get the size of the string (static method)." },
    { "copy", (PyCFunction)CString_copy_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "copy(cvm: CVirtualMachine, src: int, *, nothrow: bool = False) -> int | None\n\nCopy a string (static method)." },
    { "strncpy", (PyCFunction)CString_strncpy_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "strcpy(cvm: CVirtualMachine, dest: int, src: int, *, nothrow: bool = False) -> None\n\nCopy one string to another (static method)." },
    { "strcat", (PyCFunction)CString_strcat_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "strcat(cvm: CVirtualMachine, dest: int, src: int, *, nothrow: bool = False) -> None\n\nConcatenate two strings (static method)." },
    { "strrchr", (PyCFunction)CString_strrchr_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "strrchr(cvm: CVirtualMachine, src: int, match: int, *, nothrow: bool = False) -> int | None\n\nFind the last occurrence of a character string (static method)." },
    { "strchr", (PyCFunction)CString_strchr_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "strchr(cvm: CVirtualMachine, src: int, match: int, *, nothrow: bool = False) -> int | None\n\nFind the first occurrence of a character string (static method)." },
    { "match", (PyCFunction)CString_match_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "match(cvm: CVirtualMachine, src: int, match: int, *, nothrow: bool = False) -> int | None\n\nMatch a string with another (static method)." },
    { "match_offset", (PyCFunction)CString_match_offset_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "offset(cvm: CVirtualMachine, src: int, match: int, *, nothrow: bool = False) -> int | None\n\nCalculate the offset of a match in a string (static method)." },
    { "put_char", (PyCFunction)CString_put_char_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "put_char(cvm: CVirtualMachine, ptr: int, value: int, *, nothrow: bool = False) -> None\n\nPut a character into a string (static method)." },
    { "unsafe", (PyCFunction)CString_unsafe_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Get the raw string data as a Python string" },
    { "lessthan", (PyCFunction)CString_lessthan_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "lessthan(cvm: CVirtualMachine, src: int, dest: int, *, nothrow: bool = False) -> int | None\n\nCheck if one string is less than another (static method)." },
    { "greaterthan", (PyCFunction)CString_greaterthan_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "greaterthan(cvm: CVirtualMachine, src: int, dest: int, *, nothrow: bool = False) -> int | None\n\nCheck if one string is greater than another (static method)." },
    { "equals", (PyCFunction)CString_equals_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "equals(cvm: CVirtualMachine, src: int, dest: int, *, nothrow: bool = False) -> int | None\n\nCheck if two strings are equal (static method)." },
    { "compare", (PyCFunction)CString_compare_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "compare(cvm: CVirtualMachine, src: int, dest: int, *, nothrow: bool = False) -> int | None\n\nCompare two strings (static method)." },
    { "left", (PyCFunction)CString_left_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "left(cvm: CVirtualMachine, src: int, offset: int, *, nothrow: bool = False) -> int | None\n\nGet the left part of a string (static method)." },
    { "right", (PyCFunction)CString_right_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "right(cvm: CVirtualMachine, src: int, offset: int, *, nothrow: bool = False) -> int | None\n\nGet the right part of a string (static method)." },
    { "left_copy", (PyCFunction)CString_left_copy_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "left_copy(cvm: CVirtualMachine, src: int, nbytes: int, *, nothrow: bool = False) -> int | None\n\nCopy n bytes from the left of a string (static method)." },
    { "right_copy", (PyCFunction)CString_right_copy_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "strncpy(cvm: CVirtualMachine, src: int, nbytes: int, *, nothrow: bool = False) -> int | None\n\nCopy n bytes of a string (static method)." },
    { "move_left", (PyCFunction)CString_move_left_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "move_left(cvm: CVirtualMachine, src: int, nbytes: int, *, nothrow: bool = False) -> None\n\nMove a string left in memory (static method)." },
    { "move_right", (PyCFunction)CString_move_right_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "move_right(cvm: CVirtualMachine, src: int, nbytes: int, *, nothrow: bool = False) -> None\n\nMove a string right in memory (static method)." },
    { "strcmp", (PyCFunction)CString_strcmp_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "strcmp(cvm: CVirtualMachine, src: int, dest: int, *, nothrow: bool = False) -> int | None\n\nCompare two strings lexicographically (static method)." },
    { "ref", (PyCFunction)CString_ref_static, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "ref(cvm: CVirtualMachine, src: int, depth: int, *, nothrow: bool = False) -> int | None\n\nCompare two strings lexicographically (static method)." },

    /* CString context */
    { "__enter__", (PyCFunction)CString_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CString_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CStringTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CString",
    .tp_doc = "CString implementation in C module",
    .tp_basicsize = sizeof(CStringType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CString_new,
    .tp_init = (initproc)CString_init,
    .tp_dealloc = (destructor)CString_dealloc,
    .tp_methods = CString_methods,
};

int init_cstring(PyObject* module) {
    if (PyType_Ready(&CStringTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CStringTypeObject);
    if (PyModule_AddObject(module, "CString", (PyObject*)&CStringTypeObject) < 0) {
        Py_DECREF(&CStringTypeObject);
        return -1;
    }

    return 0;
}
