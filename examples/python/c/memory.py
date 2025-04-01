# c/memory.py
import ctypes

class CMemory:
    class memory_methods(ctypes.Structure):
        _fields_ = [
            ("alloc", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("realloc", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("free", ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
        ]

    @classmethod
    def setup(cls, lib):
        cls.memory_methods_ptr = lib.memory
        cls.memory_methods_ptr.restype = ctypes.POINTER(cls.memory_methods)
        cls.memory_methods = cls.memory_methods_ptr().contents

    @classmethod
    def alloc(self, size: ctypes.c_uint64) -> ctypes.c_void_p:
        return self.memory_methods.alloc(size)
        

    @classmethod
    def realloc(self, ptr: ctypes.c_void_p, size: ctypes.c_uint64, new_size: ctypes.c_uint64) -> ctypes.c_void_p:
        return self.memory_methods.copy(ptr, size, new_size)

 
    @classmethod
    def free(self, ptr: ctypes.c_void_p, size: ctypes.c_uint64):
        return self.memory_methods.free(ptr, size)
