# c/string.py
import ctypes
from .vm import CVirtualMachine
from .error import CException, CVirtualMachineNotInitializedException


class CString:
    class string_methods(ctypes.Structure):
        _fields_ = [
            ("free", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("copy", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("strcpy", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("strcat", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("strrchr", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("strchr", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("match", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("offset", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("load", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_char_p)),
            ("put_char", ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_char)),
            ("unsafe", ctypes.CFUNCTYPE(ctypes.c_char_p, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("size", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("lessthan", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("greaterthan", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("equals", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("compare", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("left", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("strncpy", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("left_strncpy", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("right", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("move_left", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("move_right", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("strcmp", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
        ]

    @classmethod
    def setup(cls, lib):
        cls.string_methods_ptr = lib.string
        cls.string_methods_ptr.restype = ctypes.POINTER(cls.string_methods)
        cls.string_methods = cls.string_methods_ptr().contents


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
    def free(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.free(self.vm, ptr)


    @exception_handler
    def copy(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.copy(self.vm, ptr)


    @exception_handler
    def strcpy(self, dest: ctypes.c_uint64, src: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.strcpy(self.vm, dest, src)


    @exception_handler
    def strcat(self, dest: ctypes.c_uint64, src: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.strcat(self.vm, dest, src)


    @exception_handler
    def strrchr(self, src_ptr: ctypes.c_uint64, match_ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.strrchr(self.vm, src_ptr, match_ptr)


    @exception_handler
    def strchr(self, src_ptr: ctypes.c_uint64, match_ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.strchr(self.vm, src_ptr, match_ptr)


    @exception_handler
    def match(self, src_ptr: ctypes.c_uint64, match_ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.match(self.vm, src_ptr, match_ptr)


    @exception_handler
    def offset(self, src_ptr: ctypes.c_uint64, match_ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.offset(self.vm, src_ptr, match_ptr)


    @exception_handler
    def load(self, data: ctypes.c_char_p) -> ctypes.c_uint64:
        return self.string_methods.load(self.vm, data)


    @exception_handler
    def put_char(self, string_ptr: ctypes.c_uint64, char: ctypes.c_char) -> ctypes.c_uint64:
        return self.string_methods.put_char(self.vm, string_ptr, char)

 
    @exception_handler
    def unsafe(self, string_ptr: ctypes.c_uint64) -> ctypes.c_char_p:
        return self.string_methods.unsafe(self.vm, string_ptr)


    @exception_handler
    def size(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.size(self.vm, ptr)


    @exception_handler
    def lessthan(self, src: ctypes.c_uint64, dest: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.lessthan(self.vm, src, dest)


    @exception_handler
    def greaterthan(self, src: ctypes.c_uint64, dest: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.greaterthan(self.vm, src, dest)


    @exception_handler
    def equals(self, src: ctypes.c_uint64, dest: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.equals(self.vm, src, dest)


    @exception_handler
    def compare(self, src: ctypes.c_uint64, dest: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.compare(self.vm, src, dest)


    @exception_handler
    def left(self, src: ctypes.c_uint64, offset: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.left(self.vm, src, offset)


    @exception_handler
    def strncpy(self, src: ctypes.c_uint64, nbytes: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.strncpy(self.vm, src, nbytes)


    @exception_handler
    def left_strncpy(self, src: ctypes.c_uint64, offset: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.left_strncpy(self.vm, src, offset)


    @exception_handler
    def right(self, src: ctypes.c_uint64, offset: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.right(self.vm, src, offset)


    @exception_handler
    def move_left(self, src: ctypes.c_uint64, nbytes: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.move_left(self.vm, src, nbytes)


    @exception_handler
    def strcmp(self, src: ctypes.c_uint64, dest: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.string_methods.strcmp(self.vm, src, dest)
