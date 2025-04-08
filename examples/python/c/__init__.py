# c/__init__.py
import ctypes
import os

from ._error import *
from ._memory import *
from ._os import *
from ._env import * 
from ._pointer import * 
from ._data import * 
from ._file import * 
from ._string import * 
from ._vm import * 
from ._stack import *

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


# Load the necessary C libraries.
lib_c_sys = load("sys")  # System-level functions (os, memory, error, hashtable)
lib_c_vm = load("vm")    # Virtual machine-related functions (data, env, file, pointer, string, vm)

# Check if libraries were loaded successfully.
if lib_c_sys is None or lib_c_vm is None:
    raise RuntimeError("Failed to load one or more C libraries.")

# Set up the Python classes by linking them to the C library functions.
CError.setup(lib_c_sys)
CMemory.setup(lib_c_sys)
COperatingSystem.setup(lib_c_sys)

CStack.setup(lib_c_vm)
CData.setup(lib_c_vm)
CEnvironment.setup(lib_c_vm)
CFile.setup(lib_c_vm)
CPointer.setup(lib_c_vm)
CString.setup(lib_c_vm)
CVirtualMachine.setup(lib_c_vm)
