#include "cdata.h"
#include "cvm.h"
#include "cexception.h"

static PyObject* CData_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CDataTypePtr self;
    self = (CDataTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
    }
    return (PyObject*)self;
}

static int CData_init(CDataTypePtr self, PyObject* args, PyObject* kwds) {
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

static void CData_dealloc(CDataTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CData_alloc(CDataTypePtr self, PyObject* args) {
    u64 size;
    if (!PyArg_ParseTuple(args, "K", &size)) {
        return NULL;
    }

    u64 address = CALL(data)->alloc(self->cvm, size);
    if (!address) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(address);
}

static PyObject* CData_size(CDataTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 size = CALL(data)->size(self->cvm, address);
    if (!size) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(size);
}

static PyObject* CData_unsafe(CDataTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    void_ptr data = CALL(data)->unsafe(self->cvm, address);
    if (data == NULL) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromVoidPtr(data);
}

static PyObject* CData_free(CDataTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 result = CALL(data)->free(self->cvm, address);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CData_enter(CDataTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CData_exit(CDataTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CData_methods[] = {
    { "alloc", (PyCFunction)CData_alloc, METH_VARARGS, "Allocate a data block" },
    { "size", (PyCFunction)CData_size, METH_VARARGS, "Get the size of a data block" },
    { "unsafe", (PyCFunction)CData_unsafe, METH_VARARGS, "Get a pointer to the data block" },
    { "free", (PyCFunction)CData_free, METH_VARARGS, "Free a data block" },
    { "__enter__", (PyCFunction)CData_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CData_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CDataTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CData",
    .tp_doc = "CData implementation in C module",
    .tp_basicsize = sizeof(CDataType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CData_new,
    .tp_init = (initproc)CData_init,
    .tp_dealloc = (destructor)CData_dealloc,
    .tp_methods = CData_methods,
};

int init_cdata(PyObject* module) {
    if (PyType_Ready(&CDataTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CDataTypeObject);
    if (PyModule_AddObject(module, "CData", (PyObject*)&CDataTypeObject) < 0) {
        Py_DECREF(&CDataTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
