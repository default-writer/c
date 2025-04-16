/* SPDX-License-Identifier: BSD-3-Clause */
/*-*-coding:utf-8 -*-
 * Auto updated?
 *   Yes
 * Created:
 *   April 16, 2025 at 11:03:49 AM GMT+3
 * Modified:
 *   April 16, 2025 at 6:37:47 PM GMT+3
 *
 */
/*
    Copyright (C) 2022-2047 Artur Mustafin (artur.mustafin@gmail.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
