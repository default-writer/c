# c/__init__.py
import ctypes
import os as _os
from .error import *
from .memory import *
from .os import *
from .env import *
from .pointer import *
from .data import *
from .file import *
from .string import *
from .vm import *


def load(name):
    """
    Loads a shared library (C library) with the given name.

    Args:
        name: The name of the library (without the 'libc-' prefix or '.so' suffix).

    Returns:
        A ctypes.CDLL object representing the loaded library, or None if an error occurs.
    """
    lib_path = _os.path.join("lib", f"libc-{name}.so")
    try:
        c = ctypes.CDLL(lib_path)
        return c
    except OSError as e:
        print(f"error: {e}")
        return None


# Load the necessary C libraries.
c_sys = load("sys")  # System-level functions (os, memory, error, hashtable)
c_vm = load("vm")    # Virtual machine-related functions (data, env, file, pointer, string, vm)

# Check if libraries were loaded successfully.
if c_sys is None or c_vm is None:
    raise RuntimeError("Failed to load one or more C libraries.")

# Set up the Python classes by linking them to the C library functions.
CError.setup(c_sys)
CMemory.setup(c_sys)
COperatingSystem.setup(c_sys)

CData.setup(c_vm)
CEnvironment.setup(c_vm)
CFile.setup(c_vm)
CPointer.setup(c_vm)
CString.setup(c_vm)
CVirtualMachine.setup(c_vm)
