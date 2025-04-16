/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 16, 2025 at 11:03:49 AM GMT+3
 * Modified:
 *   April 16, 2025 at 6:39:21 PM GMT+3
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

#include "cvm.h"
#include "cexception.h"

static PyObject* CVirtualMachine_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CVirtualMachineTypePtr self;
    self = (CVirtualMachineTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
    }
    return (PyObject*)self;
}

static int CVirtualMachine_init(CVirtualMachineTypePtr self, PyObject* args, PyObject* kwds) {
    u64 size;
    if (!PyArg_ParseTuple(args, "K", &size)) {
        return -1;
    }

    self->cvm = CALL(vm)->init(size);
    if (self->cvm == NULL) {
        PyErr_SetString(CVirtualMachineNotInitializedException, "Failed to initialize the virtual machine");
        return -1;
    }

    return 0;
}

static void CVirtualMachine_dealloc(CVirtualMachineTypePtr self) {
    if (self->cvm != NULL) {
        CALL(vm)->destroy(self->cvm);
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CVirtualMachine_gc(CVirtualMachineTypePtr self, PyObject* Py_UNUSED(ignored)) {
    if (self->cvm == NULL) {
        PyErr_SetString(CVirtualMachineNotInitializedException, "Virtual machine is not initialized");
        return NULL;
    }

    CALL(vm)->gc(self->cvm);

    Py_RETURN_NONE;
}

static PyObject* CVirtualMachine_release(CVirtualMachineTypePtr self, PyObject* args) {
    u64 ptr;
    if (!PyArg_ParseTuple(args, "K", &ptr)) {
        return NULL;
    }

    if (self->cvm == NULL) {
        PyErr_SetString(CVirtualMachineNotInitializedException, "Virtual machine is not initialized");
        return NULL;
    }
    u64 result = CALL(vm)->release(self->cvm, ptr);
    if (!result) {
        switch (CALL(error)->type()) {
            case ID_ERROR_NO_ERROR:
                break;
            case ID_ERROR_VM_NOT_INITIALIZED:
                PyErr_SetString(CVirtualMachineNotInitializedException, CALL(error)->get());
                break;
            case ID_ERROR_INVALID_POINTER:
                PyErr_SetString(CInvalidPointerException, CALL(error)->get());
                break;
            case ID_ERROR_INVALID_ARGUMENT:
                PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
                break;
            case ID_ERROR_INVALID_TYPE_ID:
                PyErr_SetString(CInvalidTypeIdException, CALL(error)->get());
                break;
            case ID_ERROR_INVALID_VALUE:
                PyErr_SetString(CInvalidValueException, CALL(error)->get());
                break;
            default:
                PyErr_SetString(CException, CALL(error)->get());
                break;
        }
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CVirtualMachine_dump_ref(CVirtualMachineTypePtr self, PyObject* Py_UNUSED(ignored)) {
    if (self->cvm == NULL) {
        PyErr_SetString(CVirtualMachineNotInitializedException, "Virtual machine is not initialized");
        return NULL;
    }

    CALL(vm)->dump_ref(self->cvm);

    Py_RETURN_NONE;
}

static PyObject* CVirtualMachine_dump_ref_stack(CVirtualMachineTypePtr self, PyObject* args) {
    PyObject* stack_obj;
    if (!PyArg_ParseTuple(args, "O", &stack_obj)) {
        return NULL;
    }

    stack_ptr stack = (stack_ptr)PyLong_AsVoidPtr(stack_obj);
    if (self->cvm == NULL) {
        PyErr_SetString(CVirtualMachineNotInitializedException, "Virtual machine is not initialized");
        return NULL;
    }

    CALL(vm)->dump_ref_stack(self->cvm, stack);

    Py_RETURN_NONE;
}

static PyObject* CVirtualMachine_enter(CVirtualMachineTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CVirtualMachine_exit(CVirtualMachineTypePtr self, PyObject* args) {
    if (self->cvm != NULL) {
        CALL(vm)->destroy(self->cvm);
        self->cvm = NULL;
    }
    Py_RETURN_NONE;
}

static PyMethodDef CVirtualMachine_methods[] = {
    { "gc", (PyCFunction)CVirtualMachine_gc, METH_NOARGS, "Perform garbage collection" },
    { "release", (PyCFunction)CVirtualMachine_release, METH_VARARGS, "Release a pointer" },
    { "dump_ref", (PyCFunction)CVirtualMachine_dump_ref, METH_NOARGS, "Dump references" },
    { "dump_ref_stack", (PyCFunction)CVirtualMachine_dump_ref_stack, METH_VARARGS, "Dump references for a stack" },
    { "__enter__", (PyCFunction)CVirtualMachine_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CVirtualMachine_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CVirtualMachineTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CVirtualMachine",
    .tp_doc = "CVirtualMachine implementation in C module",
    .tp_basicsize = sizeof(CVirtualMachineType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CVirtualMachine_new,
    .tp_init = (initproc)CVirtualMachine_init,
    .tp_dealloc = (destructor)CVirtualMachine_dealloc,
    .tp_methods = CVirtualMachine_methods,
};

int init_cvm(PyObject* module) {
    if (PyType_Ready(&CVirtualMachineTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CVirtualMachineTypeObject);
    if (PyModule_AddObject(module, "CVirtualMachine", (PyObject*)&CVirtualMachineTypeObject) < 0) {
        Py_DECREF(&CVirtualMachineTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
