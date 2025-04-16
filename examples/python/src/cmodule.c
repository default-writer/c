#include <Python.h>

#include "cdata.h"
#include "cenvironment.h"
#include "cexception.h"
#include "cfile.h"
#include "clist.h"
#include "cobject.h"
#include "cpointer.h"
#include "cstack.h"
#include "cstring.h"
#include "cstringreference.h"
#include "cuser.h"
#include "cvirtual.h"
#include "cvm.h"

static PyModuleDef cmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "c",
    .m_doc = "c module",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_c(void) {
    PyObject* m;

    m = PyModule_Create(&cmodule);
    if (m == NULL) {
        return NULL;
    }

    if (init_cvm(m) < 0) {
        return NULL;
    }

    if (init_clist(m) < 0) {
        return NULL;
    }

    if (init_cexception(m) < 0) {
        return NULL;
    }

    if (init_cpointer(m) < 0) {
        return NULL;
    }

    if (init_cenvironment(m) < 0) {
        return NULL;
    }

    if (init_cvirtual(m) < 0) {
        return NULL;
    }

    if (init_cdata(m) < 0) {
        return NULL;
    }

    if (init_cfile(m) < 0) {
        return NULL;
    }

    if (init_cobject(m) < 0) {
        return NULL;
    }

    if (init_cstack(m) < 0) {
        return NULL;
    }

    if (init_cstring(m) < 0) {
        return NULL;
    }

    if (init_cstringreference(m) < 0) {
        return NULL;
    }

    if (init_cuser(m) < 0) {
        return NULL;
    }

    return m;
}
