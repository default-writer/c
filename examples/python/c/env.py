# c/env.py
import ctypes
from .vm import CVirtualMachine
from .error import CException, CVirtualMachineNotInitializedException


class CEnvironment:
    class env_methods(ctypes.Structure):
        _fields_ = [
            ("getenv", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("getcwd", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p))),
            ("puts", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
        ]

    @classmethod
    def setup(cls, lib):
        cls.env_methods_ptr = lib.env
        cls.env_methods_ptr.restype = ctypes.POINTER(cls.env_methods)
        cls.env_methods = cls.env_methods_ptr().contents


    def __init__(self, vm: CVirtualMachine):
        if vm is None:
            raise CVirtualMachineNotInitializedException("VM is not initialized for this String instance.")
        self.vm = vm.ptr


    def exception_handler(func):
        def wrapper(self, *args, **kwargs):
            result = func(self, *args, **kwargs)
            CException.check()
            return result
        return wrapper


    @exception_handler
    def getenv(self, name: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.env_methods.getenv(self.vm, name)


    @exception_handler
    def getcwd(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.env_methods.getcwd(self.vm)


    @exception_handler
    def puts(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.env_methods.puts(self.vm, ptr)
