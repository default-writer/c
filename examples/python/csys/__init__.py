# c/__init__.py
import ctypes
import os
from ._error import *
from ._memory import *
from ._os import *


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


lib_c_sys = load("sys")
if lib_c_sys is None:
    raise RuntimeError("Failed to load one or more C libraries.")

CError.setup(lib_c_sys)
CMemory.setup(lib_c_sys)
COperatingSystem.setup(lib_c_sys)
