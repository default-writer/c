#include "cenvironment.h"
#include "cvm.h"
#include "cexception.h"

static PyObject* CEnvironment_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CEnvironmentTypePtr self;
    self = (CEnvironmentTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
    }
    return (PyObject*)self;
}

static int CEnvironment_init(CEnvironmentTypePtr self, PyObject* args, PyObject* kwds) {
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

static void CEnvironment_dealloc(CEnvironmentTypePtr self) {
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CEnvironment_getenv(CEnvironmentTypePtr self, PyObject* args) {
    u64 name_ptr;
    if (!PyArg_ParseTuple(args, "K", &name_ptr)) {
        return NULL;
    }

    u64 result = CALL(env)->getenv(self->cvm, name_ptr);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CEnvironment_getcwd(CEnvironmentTypePtr self, PyObject* Py_UNUSED(ignored)) {
    u64 result = CALL(env)->getcwd(self->cvm);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CEnvironment_puts(CEnvironmentTypePtr self, PyObject* args) {
    u64 address;
    if (!PyArg_ParseTuple(args, "K", &address)) {
        return NULL;
    }

    u64 result = CALL(env)->puts(self->cvm, address);
    if (!result) {
        PyErr_SetString(CInvalidPointerException, CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CEnvironment_enter(CEnvironmentTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CEnvironment_exit(CEnvironmentTypePtr self, PyObject* args) {
    Py_RETURN_NONE;
}

static PyMethodDef CEnvironment_methods[] = {
    { "getenv", (PyCFunction)CEnvironment_getenv, METH_VARARGS, "Retrieve an environment variable" },
    { "getcwd", (PyCFunction)CEnvironment_getcwd, METH_NOARGS, "Get the current working directory" },
    { "puts", (PyCFunction)CEnvironment_puts, METH_VARARGS, "Print a string to the standard output" },
    { "__enter__", (PyCFunction)CEnvironment_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CEnvironment_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CEnvironmentTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CEnvironment",
    .tp_doc = "CEnvironment implementation in C module",
    .tp_basicsize = sizeof(CEnvironmentType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CEnvironment_new,
    .tp_init = (initproc)CEnvironment_init,
    .tp_dealloc = (destructor)CEnvironment_dealloc,
    .tp_methods = CEnvironment_methods,
};

int init_cenvironment(PyObject* module) {
    if (PyType_Ready(&CEnvironmentTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CEnvironmentTypeObject);
    if (PyModule_AddObject(module, "CEnvironment", (PyObject*)&CEnvironmentTypeObject) < 0) {
        Py_DECREF(&CEnvironmentTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
