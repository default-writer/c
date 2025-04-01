# c/file.py
import ctypes
from .vm import CVirtualMachine
from .error import CException, CVirtualMachineNotInitializedException


class CFile:
    class file_methods(ctypes.Structure):
        _fields_ = [
            ("alloc", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("data", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("free", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
        ]

    @classmethod
    def setup(cls, lib):
        cls.file_methods_ptr = lib.file
        cls.file_methods_ptr.restype = ctypes.POINTER(cls.file_methods)
        cls.file_methods = cls.file_methods_ptr().contents


    def __init__(self, vm: CVirtualMachine):
        if vm is None:
            raise CVirtualMachineNotInitializedException("VM is not initialized for this CFile instance.")
        self.vm = vm.ptr


    def exception_handler(func):
        def wrapper(self, *args, **kwargs):
            result = func(self, *args, **kwargs)
            CException.check()
            return result
        return wrapper


    @exception_handler
    def alloc(self, file_path_ptr: ctypes.c_uint64, mode_ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.file_methods.alloc(self.vm, file_path_ptr, mode_ptr)


    @exception_handler
    def data(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.file_methods.data(self.vm, ptr)


    @exception_handler
    def free(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.file_methods.free(self.vm, ptr)

