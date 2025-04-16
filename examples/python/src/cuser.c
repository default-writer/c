#include "cuser.h"
#include "cvm.h"
#include "cexception.h"

static PyObject* CUser_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CUserTypePtr self;
    self = (CUserTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
        self->user_ptr = 0;
    }
    return (PyObject*)self;
}

static int CUser_init(CUserTypePtr self, PyObject* args, PyObject* kwds) {
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

    self->user_ptr = CALL(user)->alloc(self->cvm);
    if (self->user_ptr == 0) {
        PyErr_SetString(CInvalidPointerException, "Failed to allocate user object");
        return -1;
    }

    return 0;
}

static void CUser_dealloc(CUserTypePtr self) {
    if (self->user_ptr != 0) {
        CALL(user)->free(self->cvm, self->user_ptr);
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CUser_free(CUserTypePtr self, PyObject* Py_UNUSED(ignored)) {
    if (self->user_ptr != 0) {
        if (!CALL(user)->free(self->cvm, self->user_ptr)) {
            PyErr_SetString(CInvalidPointerException, CALL(error)->get());
            return NULL;
        }
        self->user_ptr = 0;
    }
    Py_RETURN_NONE;
}

static PyObject* CUser_enter(CUserTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CUser_exit(CUserTypePtr self, PyObject* args) {
    if (self->user_ptr != 0) {
        CALL(user)->free(self->cvm, self->user_ptr);
    }
    Py_RETURN_NONE;
}

static PyMethodDef CUser_methods[] = {
    { "free", (PyCFunction)CUser_free, METH_NOARGS, "Free the user object" },
    { "__enter__", (PyCFunction)CUser_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CUser_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CUserTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CUser",
    .tp_doc = "CUser implementation in C module",
    .tp_basicsize = sizeof(CUserType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CUser_new,
    .tp_init = (initproc)CUser_init,
    .tp_dealloc = (destructor)CUser_dealloc,
    .tp_methods = CUser_methods,
};

int init_cuser(PyObject* module) {
    if (PyType_Ready(&CUserTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CUserTypeObject);
    if (PyModule_AddObject(module, "CUser", (PyObject*)&CUserTypeObject) < 0) {
        Py_DECREF(&CUserTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
