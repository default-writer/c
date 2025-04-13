#include <Python.h>

#include "std/data.h"

#include "virtual/list/list_v1.h"

typedef struct {
    PyObject_HEAD;
    stack_ptr stack; // Use stack_ptr from list_v1.h
    const_vm_ptr cvm; // Pointer to the virtual machine
} CList;

static PyObject* CList_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CList* self;
    self = (CList*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
        self->stack = NULL;
    }
    return (PyObject*)self;
}

static int CList_init(CList* self, PyObject* args, PyObject* kwds) {
    PyObject* cvm_obj;
    if (!PyArg_ParseTuple(args, "O", &cvm_obj)) {
        return -1;
    }

    // Check if cvm_obj is an instance of CVirtualMachine
    PyObject* cvm_type = PyObject_GetAttrString((PyObject*)Py_TYPE(cvm_obj), "__name__");
    if (cvm_type == NULL || strcmp(PyUnicode_AsUTF8(cvm_type), "CVirtualMachine") != 0) {
        PyErr_SetString(PyExc_TypeError, "Expected a CVirtualMachine instance");
        Py_XDECREF(cvm_type);
        return -1;
    }
    Py_XDECREF(cvm_type);

    // Extract the virtual machine pointer
    self->cvm = (const_vm_ptr)PyLong_AsVoidPtr(cvm_obj);
    if (self->cvm == NULL) {
        PyErr_SetString(PyExc_ValueError, "Invalid CVirtualMachine pointer");
        return -1;
    }

    // Initialize the stack using CALL(list)->init
    self->stack = CALL(list)->init(self->cvm);
    if (self->stack == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to initialize the list");
        return -1;
    }

    return 0;
}

static void CList_dealloc(CList* self) {
    if (self->stack != NULL) {
        // Destroy the stack using CALL(list)->destroy
        CALL(list)->destroy(self->cvm, self->stack);
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CList_push(CList* self, PyObject* args) {
    void_ptr data;
    if (!PyArg_ParseTuple(args, "k", &data)) { // Parse an unsigned long (void_ptr)
        return NULL;
    }

    // Push the data onto the stack using CALL(list)->push
    if (!CALL(list)->push(self->cvm, self->stack, data)) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to push data onto the list");
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject* CList_pop(CList* self, PyObject* Py_UNUSED(ignored)) {
    // Pop data from the stack using CALL(list)->pop
    void_ptr data = CALL(list)->pop(self->cvm, self->stack);
    if (data == NULL) {
        Py_RETURN_NONE;
    }

    return PyLong_FromVoidPtr(data);
}

static PyObject* CList_peek(CList* self, PyObject* Py_UNUSED(ignored)) {
    // Peek at the top of the stack using CALL(list)->peek
    void_ptr data = CALL(list)->peek(self->cvm, self->stack);
    if (data == NULL) {
        Py_RETURN_NONE;
    }

    return PyLong_FromVoidPtr(data);
}

/* Implement __enter__ and __exit__ for context management */
static PyObject* CList_enter(CList* self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self); // Increment reference count to keep the object alive
    return (PyObject*)self;
}

static PyObject* CList_exit(CList* self, PyObject* args) {
    // Clean up resources when exiting the context
    CList_dealloc(self);
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

static PyTypeObject CListType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "clist.CList",
    .tp_doc = "clist implementation in C",
    .tp_basicsize = sizeof(CList),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CList_new,
    .tp_init = (initproc)CList_init,
    .tp_dealloc = (destructor)CList_dealloc,
    .tp_methods = CList_methods,
};

static PyModuleDef clistmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "clist",
    .m_doc = "clist module",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_clist(void) {
    PyObject* m;
    if (PyType_Ready(&CListType) < 0)
        return NULL;

    m = PyModule_Create(&clistmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&CListType);
    if (PyModule_AddObject(m, "CList", (PyObject*)&CListType) < 0) {
        Py_DECREF(&CListType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
