#define USING_CMODULE

#ifndef CMODULE_H
#define CMODULE_H

#include <Python.h>

#ifdef PyVarObject_HEAD_INIT
#undef PyVarObject_HEAD_INIT
#define PyVarObject_HEAD_INIT(type, size) \
    {                                     \
        PyObject_HEAD_INIT(type)(size)    \
    }
#endif

#endif // CMODULE_H
