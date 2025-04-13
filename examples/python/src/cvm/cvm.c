#include <Python.h>

#include "std/data.h"

#include "virtual/vm/vm_v1.h" // Include the header for vm_v1 methods

typedef struct {
    PyObject_HEAD;
    const_vm_ptr cvm; // Pointer to the virtual machine
} CVirtualMachine;

static PyObject* CVirtualMachine_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    CVirtualMachine* self;
    self = (CVirtualMachine*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cvm = NULL;
    }
    return (PyObject*)self;
}

static int CVirtualMachine_init(CVirtualMachine* self, PyObject* args, PyObject* kwds) {
    u64 size;
    if (!PyArg_ParseTuple(args, "K", &size)) { // Parse an unsigned long long (u64)
        return -1;
    }

    // Initialize the virtual machine using CALL(vm)->init
    self->cvm = CALL(vm)->init(size);
    if (self->cvm == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to initialize the virtual machine");
        return -1;
    }

    return 0;
}

static void CVirtualMachine_dealloc(CVirtualMachine* self) {
    if (self->cvm != NULL) {
        // Destroy the virtual machine using CALL(vm)->destroy
        CALL(vm)->destroy(self->cvm);
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* CVirtualMachine_gc(CVirtualMachine* self, PyObject* Py_UNUSED(ignored)) {
    if (self->cvm == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Virtual machine is not initialized");
        return NULL;
    }

    // Perform garbage collection using CALL(vm)->gc
    CALL(vm)->gc(self->cvm);

    Py_RETURN_NONE;
}

static PyObject* CVirtualMachine_release(CVirtualMachine* self, PyObject* args) {
    u64 ptr;
    if (!PyArg_ParseTuple(args, "K", &ptr)) { // Parse an unsigned long long (u64)
        return NULL;
    }

    if (self->cvm == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Virtual machine is not initialized");
        return NULL;
    }

    // Release the pointer using CALL(vm)->release
    u64 result = CALL(vm)->release(self->cvm, ptr);
    return PyLong_FromUnsignedLongLong(result);
}

static PyObject* CVirtualMachine_dump_ref(CVirtualMachine* self, PyObject* Py_UNUSED(ignored)) {
    if (self->cvm == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Virtual machine is not initialized");
        return NULL;
    }

    // Dump references using CALL(vm)->dump_ref
    CALL(vm)->dump_ref(self->cvm);

    Py_RETURN_NONE;
}

static PyObject* CVirtualMachine_dump_ref_stack(CVirtualMachine* self, PyObject* args) {
    PyObject* stack_obj;
    if (!PyArg_ParseTuple(args, "O", &stack_obj)) {
        return NULL;
    }

    stack_ptr stack = (stack_ptr)PyLong_AsVoidPtr(stack_obj);
    if (self->cvm == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Virtual machine is not initialized");
        return NULL;
    }

    // Dump references for the stack using CALL(vm)->dump_ref_stack
    CALL(vm)->dump_ref_stack(self->cvm, stack);

    Py_RETURN_NONE;
}

/* Implement __enter__ and __exit__ for context management */
static PyObject* CVirtualMachine_enter(CVirtualMachine* self, PyObject* Py_UNUSED(ignored)) {
    Py_INCREF(self); // Increment reference count to keep the object alive
    return (PyObject*)self;
}

static PyObject* CVirtualMachine_exit(CVirtualMachine* self, PyObject* args) {
    // Clean up resources when exiting the context
    CVirtualMachine_dealloc(self);
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

static PyTypeObject CVirtualMachineType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "cvm.CVirtualMachine",
    .tp_doc = "cvm implementation in C",
    .tp_basicsize = sizeof(CVirtualMachine),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = CVirtualMachine_new,
    .tp_init = (initproc)CVirtualMachine_init,
    .tp_dealloc = (destructor)CVirtualMachine_dealloc,
    .tp_methods = CVirtualMachine_methods,
};

static PyModuleDef cvmmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "cvm",
    .m_doc = "cvm module",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_cvm(void) {
    PyObject* m;
    if (PyType_Ready(&CVirtualMachineType) < 0)
        return NULL;

    m = PyModule_Create(&cvmmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&CVirtualMachineType);
    if (PyModule_AddObject(m, "CVirtualMachine", (PyObject*)&CVirtualMachineType) < 0) {
        Py_DECREF(&CVirtualMachineType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
