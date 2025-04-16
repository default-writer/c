# c/__init__.py
import ctypes
import os
from ._data import * 
from ._env import * 
from ._file import * 
from ._list import *
from ._pointer import * 
from ._stack import *
from ._string import * 
from ._vm import * 

def load(name):
    """
    Loads a shared library (C library) with the given name.

    Args:
        name: The name of the library (without the 'libc-' prefix or '.so' suffix).

    Returns:
        A ctypes.CDLL object representing the loaded library, or None if an error occurs.
    """
    lib_path = os.path.join("lib", f"libc-{name}.so")
    try:
        c = ctypes.CDLL(lib_path)
        return c
    except OSError as e:
        print(f"error: {e}")
        return None

lib_c_vm = load("vm")
if lib_c_vm is None:
    raise RuntimeError("Failed to load one or more C libraries.")

CList.setup(lib_c_vm)
CStack.setup(lib_c_vm)
CData.setup(lib_c_vm)
CEnvironment.setup(lib_c_vm)
CFile.setup(lib_c_vm)
CPointer.setup(lib_c_vm)
CString.setup(lib_c_vm)
CVirtualMachine.setup(lib_c_vm)
