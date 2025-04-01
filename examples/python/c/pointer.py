# c/pointer.py
import ctypes


class CType(ctypes.c_uint64):
    Default = 0
    Data = 1
    String = 2
    StringPointer = 3
    File = 4
    Stack = 5
    Object = 6
    User = 7


class CPointer:
    class pointer_methods(ctypes.Structure):
        _fields_ = [
            ("alloc", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("copy", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64, ctypes.c_uint64)),
            ("read", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("free", ctypes.CFUNCTYPE(None, ctypes.c_uint64, ctypes.c_uint64)),
        ]

    @classmethod
    def setup(cls, lib):
        cls.pointer_methods_ptr = lib.pointer
        cls.pointer_methods_ptr.restype = ctypes.POINTER(cls.pointer_methods)
        cls.pointer_methods = cls.pointer_methods_ptr().contents

    @classmethod
    def alloc(self, data: ctypes.c_void_p, size: ctypes.c_uint64, type_id: CType) -> ctypes.c_uint64:
        return self.pointer_methods.alloc(data, size, type_id)


    @classmethod
    def copy(self, src: ctypes.c_void_p, size: ctypes.c_uint64, offset: ctypes.c_uint64, type_id: CType) -> ctypes.c_uint64:
        return self.pointer_methods.copy(src, size, offset, type_id)

 
    @classmethod
    def read(self, address: ctypes.c_uint64, type_id: CType) -> ctypes.c_void_p:
        return self.pointer_methods.read(address, type_id)

 
    @classmethod
    def free(self, address: ctypes.c_uint64, type_id: CType) -> ctypes.c_uint64:
        return self.pointer_methods.free(address, type_id)
