#include "cpointer.h"
#include "cvm.h"
#include "cexception.h"

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

    return 0;
}

static void CPointer_dealloc(CPointerTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CPointer_alloc(CPointerTypePtr self, PyObject* args) {
    void_ptr data;
    u64 size, type_id;
    if (!PyArg_ParseTuple(args, "KkK", &data, &size, &type_id)) {
        return NULL;
    }

    u64 address = CALL(pointer)->alloc(self->cvm, data, size, type_id);
    if (!address) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(address);
}

static PyObject* CPointer_copy(CPointerTypePtr self, PyObject* args) {
    u64 src, size, offset, type_id;
    if (!PyArg_ParseTuple(args, "KKKK", &src, &size, &offset, &type_id)) {
        return NULL;
    }

    u64 address = CALL(pointer)->copy(self->cvm, (const_void_ptr)src, size, offset, type_id);
    if (!address) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(address);
}

static PyObject* CPointer_read(CPointerTypePtr self, PyObject* args) {
    u64 address, type_id;
    if (!PyArg_ParseTuple(args, "KK", &address, &type_id)) {
        return NULL;
    }

    const_void_ptr data = CALL(pointer)->read(self->cvm, address, type_id);
    if (data == NULL) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromVoidPtr((void_ptr)data);
}

static PyObject* CPointer_ref(CPointerTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 result = CALL(pointer)->ref(self->cvm, address);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CPointer_free(CPointerTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 result = CALL(pointer)->free(self->cvm, address);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CPointer_enter(CPointerTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CPointer_exit(CPointerTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CPointer_methods[] = {
    { "alloc", (PyCFunction)CPointer_alloc, METH_VARARGS, "Allocate a pointer" },
    { "copy", (PyCFunction)CPointer_copy, METH_VARARGS, "Copy a pointer" },
    { "read", (PyCFunction)CPointer_read, METH_VARARGS, "Read data from a pointer" },
    { "ref", (PyCFunction)CPointer_ref, METH_VARARGS, "Get reference count of a pointer" },
    { "free", (PyCFunction)CPointer_free, METH_VARARGS, "Free a pointer" },
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
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
