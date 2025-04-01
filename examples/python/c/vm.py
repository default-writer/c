# c/vm.py
import ctypes


class CVirtualMachine:
    class vm_methods(ctypes.Structure):
        _fields_ = [
            ("init", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("gc", ctypes.CFUNCTYPE(None)),
            ("release", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.c_uint64)),
            ("destroy", ctypes.CFUNCTYPE(None)),
        ]

    @classmethod
    def setup(cls, lib):
        cls.vm_methods_ptr = lib.vm
        cls.vm_methods_ptr.restype = ctypes.POINTER(cls.vm_methods)
        cls.vm_methods = cls.vm_methods_ptr().contents

    def __init__(self, size):
        self.ptr = self.vm_methods.init(size);
    

    def gc(self):
        return self.vm_methods.gc()
    

    def release(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        return self.vm_methods.release(ptr)
    

    def destroy(self, ptr: ctypes.c_uint64):
        return self.vm_methods.release(ptr)
    

    def __del__(self):
        return self.vm_methods.destroy()
