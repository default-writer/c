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
    lib_path = _os.path.join("lib", f"libc-{name}.so")
    try:
        c = ctypes.CDLL(lib_path)
        return c
    except OSError as e:
        print(f"error: {e}")
        return None


c_sys = load("sys")
c_vm = load("vm")


CError.setup(c_sys)
CMemory.setup(c_sys)
COperatingSystem.setup(c_sys)
CData.setup(c_vm)
CEnvironment.setup(c_vm)
CFile.setup(c_vm)
CPointer.setup(c_vm)
CString.setup(c_vm)
CVirtualMachine.setup(c_vm)

