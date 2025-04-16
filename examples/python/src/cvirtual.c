#include "cvirtual.h"
#include "cvm.h"
#include "cexception.h"

static PyObject* CVirtual_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CVirtualTypePtr self;
    self = (CVirtualTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
    }
    return (PyObject*)self;
}

static int CVirtual_init(CVirtualTypePtr self, PyObject* args, PyObject* kwds) {
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

static void CVirtual_dealloc(CVirtualTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CVirtual_alloc(CVirtualTypePtr self, PyObject* args) {
    u64 size, type_id;
    if (!PyArg_ParseTuple(args, "KK", &size, &type_id)) {
        return NULL;
    }

    u64 address = CALL(virtual)->alloc(self->cvm, size, type_id);
    if (!address) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(address);
}

static PyObject* CVirtual_read(CVirtualTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    const_pointer_ptr data = CALL(virtual)->read(self->cvm, address);
    if (data == NULL) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromVoidPtr((void_ptr)data);
}

static PyObject* CVirtual_type(CVirtualTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 type_id = CALL(virtual)->type(self->cvm, address);
    if (!type_id) {
        PyErr_SetString(CInvalidTypeIdException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(type_id);
}

static PyObject* CVirtual_free(CVirtualTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 result = CALL(virtual)->free(self->cvm, address);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CVirtual_enter(CVirtualTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CVirtual_exit(CVirtualTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CVirtual_methods[] = {
    { "alloc", (PyCFunction)CVirtual_alloc, METH_VARARGS, "Allocate memory in the virtual machine" },
    { "read", (PyCFunction)CVirtual_read, METH_VARARGS, "Read memory from the virtual machine" },
    { "type", (PyCFunction)CVirtual_type, METH_VARARGS, "Get the type of a memory address" },
    { "free", (PyCFunction)CVirtual_free, METH_VARARGS, "Free memory in the virtual machine" },
    { "__enter__", (PyCFunction)CVirtual_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CVirtual_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CVirtualTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CVirtual",
    .tp_doc = "CVirtual implementation in C module",
    .tp_basicsize = sizeof(CVirtualType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CVirtual_new,
    .tp_init = (initproc)CVirtual_init,
    .tp_dealloc = (destructor)CVirtual_dealloc,
    .tp_methods = CVirtual_methods,
};

int init_cvirtual(PyObject* module) {
    if (PyType_Ready(&CVirtualTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CVirtualTypeObject);
    if (PyModule_AddObject(module, "CVirtual", (PyObject*)&CVirtualTypeObject) < 0) {
        Py_DECREF(&CVirtualTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
