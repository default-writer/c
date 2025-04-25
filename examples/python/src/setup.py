from setuptools import setup, Extension

c_module = Extension(
    "c",
    sources=[
        "cvm.c",
        "clist.c",
        "cmodule.c",
        "cpointer.c",
        "cenvironment.c",
        "cexception.c",
        "cvirtual.c",
        "cdata.c",
        "cfile.c",
        "cobject.c",
        "cstack.c",
        "cstring.c",
        "cuser.c",
        "py_api.c",
    ],
    include_dirs=["../../../src"],
    library_dirs=["../../../lib"],
    libraries=["c-sys", "c-vm"],
)

setup(
    name="c",
    version="1.0.0",
    description="C library implementation in C module",
    ext_modules=[c_module],
)
