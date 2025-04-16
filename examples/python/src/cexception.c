#include "cexception.h"

PyObject* CException = NULL;
PyObject* CVirtualMachineNotInitializedException = NULL;
PyObject* CInvalidPointerException = NULL;
PyObject* CInvalidArgumentException = NULL;
PyObject* CInvalidTypeIdException = NULL;
PyObject* CInvalidValueException = NULL;

static PyObject* create_exception_class(const char* name, PyObject* base_class, const char* docstring) {
    PyObject* name_obj = PyUnicode_FromString(name);
    if (name_obj == NULL) {
        return NULL;
    }

    PyObject* dict = PyDict_New();
    if (dict == NULL) {
        Py_DECREF(name_obj);
        return NULL;
    }

    PyObject* docstring_obj = PyUnicode_FromString(docstring);
    if (docstring_obj == NULL) {
        Py_DECREF(name_obj);
        Py_DECREF(dict);
        return NULL;
    }

    if (PyDict_SetItemString(dict, "__doc__", docstring_obj) < 0) {
        Py_DECREF(name_obj);
        Py_DECREF(dict);
        Py_DECREF(docstring_obj);
        return NULL;
    }
    Py_DECREF(docstring_obj);

    PyObject* bases = PyTuple_Pack(1, base_class);
    if (bases == NULL) {
        Py_DECREF(name_obj);
        Py_DECREF(dict);
        return NULL;
    }

    PyObject* exception_class = PyObject_CallFunctionObjArgs((PyObject*)&PyType_Type, name_obj, bases, dict, NULL);
    Py_DECREF(name_obj);
    Py_DECREF(dict);
    Py_DECREF(bases);

    return exception_class;
}

// New method: clear
static PyObject* CException_clear(PyObject* self, PyObject* Py_UNUSED(ignored)) {
    CALL(error)->clear();
    Py_RETURN_NONE;
}

static PyMethodDef CException_methods[] = {
    { "clear", (PyCFunction)CException_clear, METH_NOARGS, "Clear the current error state" },
    { NULL, NULL, 0, NULL }
};

PyTypeObject CExceptionTypeObject = {
    PyVarObject_HEAD_INIT(NULL, 0),
    .tp_name = "c.CException",
    .tp_doc = "CException implementation in C module",
    .tp_basicsize = sizeof(PyBaseExceptionObject),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_methods = CException_methods,
};

int init_cexception(PyObject* module) {
    CExceptionTypeObject.tp_base = (PyTypeObject*)PyExc_Exception;

    if (PyType_Ready(&CExceptionTypeObject) < 0) {
        return -1;
    }

    Py_INCREF(&CExceptionTypeObject);
    if (PyModule_AddObject(module, "CException", (PyObject*)&CExceptionTypeObject) < 0) {
        Py_DECREF(&CExceptionTypeObject);
        Py_DECREF(module);
        return -1;
    }

    CException = create_exception_class("CException", (PyObject*)&CExceptionTypeObject, "CException is raised when an operation raised generic runtime exception");
    CVirtualMachineNotInitializedException = create_exception_class("CVirtualMachineNotInitializedException", (PyObject*)&CExceptionTypeObject, "CVirtualMachineNotInitializedException is raised when an operation is attempted on the VM which has not been initialized.");
    CInvalidPointerException = create_exception_class("CInvalidPointerException", (PyObject*)&CExceptionTypeObject, "CInvalidPointerException is raised when an operation is attempted with an invalid memory pointer.");
    CInvalidArgumentException = create_exception_class("CInvalidArgumentException", (PyObject*)&CExceptionTypeObject, "CInvalidArgumentException is raised when an operation is attempted with an invalid argument.");
    CInvalidTypeIdException = create_exception_class("CInvalidTypeIdException", (PyObject*)&CExceptionTypeObject, "CInvalidTypeIdException is raised when an operation is attempted with an invalid type identifier.");
    CInvalidValueException = create_exception_class("CInvalidValueException", (PyObject*)&CExceptionTypeObject, "CInvalidValueException is raised when an operation is attempted with an invalid value.");

    if (PyModule_AddObject(module, "CException", CException) < 0 ||
        PyModule_AddObject(module, "CVirtualMachineNotInitializedException", CVirtualMachineNotInitializedException) < 0 ||
        PyModule_AddObject(module, "CInvalidPointerException", CInvalidPointerException) < 0 ||
        PyModule_AddObject(module, "CInvalidArgumentException", CInvalidArgumentException) < 0 ||
        PyModule_AddObject(module, "CInvalidTypeIdException", CInvalidTypeIdException) < 0 ||
        PyModule_AddObject(module, "CInvalidValueException", CInvalidValueException) < 0) {
        Py_XDECREF(CException);
        Py_XDECREF(CVirtualMachineNotInitializedException);
        Py_XDECREF(CInvalidPointerException);
        Py_XDECREF(CInvalidArgumentException);
        Py_XDECREF(CInvalidTypeIdException);
        Py_XDECREF(CInvalidValueException);
        Py_DECREF(module);
        return -1;
    }

    return 0;
}
