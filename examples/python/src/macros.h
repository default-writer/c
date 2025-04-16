#define USING_MACROS

#ifndef MACROS_H
#define MACROS_H

#include <Python.h>

#ifdef PyVarObject_HEAD_INIT
#undef PyVarObject_HEAD_INIT
#define PyVarObject_HEAD_INIT(type, size) \
    {                                     \
        PyObject_HEAD_INIT(type)(size)    \
    }
#endif

#ifdef PyObject_HEAD
#undef PyObject_HEAD
#define PyObject_HEAD PyObject ob_base
#endif

#endif // MACROS_H
