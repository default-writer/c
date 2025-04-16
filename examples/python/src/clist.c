#include "clist.h"
#include "cvm.h"
#include "cexception.h"

static PyObject* CList_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CListTypePtr self;
    self = (CListTypePtr)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
        self->stack = NULL;
    }
    return (PyObject*)self;
}

static int CList_init(CListTypePtr self, PyObject* args, PyObject* kwds) {
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

    self->stack = CALL(list)->init(self->cvm);
    if (self->stack == NULL) {
        PyErr_SetString(CInvalidPointerException, "Failed to initialize the list");
        return -1;
    }

    return 0;
}

static void CList_dealloc(CListTypePtr self) {
    if (self->stack != NULL) {
        CALL(list)->destroy(self->cvm, self->stack);
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CList_push(CListTypePtr self, PyObject* args) {
    void_ptr data;
    if (!PyArg_ParseTuple(args, "k", &data)) {
        return NULL;
    }

    if (!CALL(list)->push(self->cvm, self->stack, data)) {
        PyErr_SetString(CInvalidArgumentException, CALL(error)->get());
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CList_pop(CListTypePtr self, PyObject* Py_UNUSED(ignored)) {
    void_ptr data = CALL(list)->pop(self->cvm, self->stack);
    if (data == NULL) {
        PyErr_SetString(CInvalidValueException, "No elements to pop from the list");
        return NULL;
    }

    return PyLong_FromVoidPtr(data);
}

static PyObject* CList_peek(CListTypePtr self, PyObject* Py_UNUSED(ignored)) {
    void_ptr data = CALL(list)->peek(self->cvm, self->stack);
    if (data == NULL) {
        PyErr_SetString(CInvalidValueException, "No elements to peek in the list");
        return NULL;
    }

    return PyLong_FromVoidPtr(data);
}

static PyObject* CList_enter(CListTypePtr self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self);
    return (PyObject*)self;
}

static PyObject* CList_exit(CListTypePtr self, PyObject* args) {
    if (self->stack != NULL) {
        CALL(list)->destroy(self->cvm, self->stack);
    }
    Py_RETURN_NONE;
}

static PyMethodDef CList_methods[] = {
    { "push", (PyCFunction)CList_push, METH_VARARGS, "Push an element onto the list" },
    { "pop", (PyCFunction)CList_pop, METH_NOARGS, "Pop an element from the list" },
    { "peek", (PyCFunction)CList_peek, METH_NOARGS, "Peek at the top element of the list" },
    { "__enter__", (PyCFunction)CList_enter, METH_NOARGS, "Enter the context" },
    { "__exit__", (PyCFunction)CList_exit, METH_VARARGS, "Exit the context" },
    { NULL } /* Sentinel */
};

PyTypeObject CListTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CList",
    .tp_doc = "CList implementation in C module",
    .tp_basicsize = sizeof(CListType),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CList_new,
    .tp_init = (initproc)CList_init,
    .tp_dealloc = (destructor)CList_dealloc,
    .tp_methods = CList_methods,
};

int init_clist(PyObject* module) {
    if (PyType_Ready(&CListTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CListTypeObject);
    if (PyModule_AddObject(module, "CList", (PyObject*)&CListTypeObject) < 0) {
        Py_DECREF(&CListTypeObject);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
