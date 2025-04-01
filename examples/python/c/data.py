# c/data.py
import ctypes
from .vm import CVirtualMachine
from .error import CException, CVirtualMachineNotInitializedException


class CData:
    class data_methods(ctypes.Structure):
        _fields_ = [
            ("alloc", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("size", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("unsafe", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("free", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
        ]

    @classmethod
    def setup(cls, lib):
        cls.data_methods_ptr = lib.data
        cls.data_methods_ptr.restype = ctypes.POINTER(cls.data_methods)
        cls.data_methods = cls.data_methods_ptr().contents


    def __init__(self, vm: CVirtualMachine):
        if vm is None:
            raise CVirtualMachineNotInitializedException("VM is not initialized for this Data instance.")
        self.vm = vm.ptr


    def exception_handler(func):
        def wrapper(self, *args, **kwargs):
            result = func(self, *args, **kwargs)
            CException.check()
            return result
        return wrapper


    @exception_handler
    def alloc(self, size: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.data_methods.alloc(self.vm, size)


    @exception_handler
    def size(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.data_methods.size(self.vm, ptr)


    @exception_handler
    def unsafe(self, ptr: ctypes.c_uint64) -> ctypes.POINTER:
        return self.data_methods.unsafe(self.vm, ptr)


    @exception_handler
    def free(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.data_methods.free(self.vm, ptr)
