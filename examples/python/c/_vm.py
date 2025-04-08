# c/vm.py
import ctypes


class CVirtualMachine:
    """
    CVirtualMachine class provides an interface to interact with a virtual machine
    in a C library.

    It allows initializing, garbage collecting, releasing resources, and destroying
    the virtual machine.
    """

    class vm_methods(ctypes.Structure):
        """
        Structure representing the function pointers for virtual machine operations in the C library.

        _fields_:
            init: Function pointer for initializing the virtual machine.
            gc: Function pointer for performing garbage collection.
            release: Function pointer for releasing a resource managed by the VM.
            destroy: Function pointer for destroying the virtual machine.
        """
        _fields_ = [
            ("init", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("gc", ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_void_p))),
            ("release", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("destroy", ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_void_p))),
        ]

    @classmethod
    def setup(cls, lib):
        """
        Sets up the CVirtualMachine class by linking it to the C library's VM functions.

        Args:
            lib: The loaded C library object (e.g., from ctypes.CDLL).

        Raises:
            AttributeError: If the C library does not have a 'vm' attribute.
        """
        cls.vm_methods_ptr = lib.vm
        cls.vm_methods_ptr.restype = ctypes.POINTER(cls.vm_methods)
        cls.vm_methods = cls.vm_methods_ptr().contents

    def __init__(self, size: ctypes.c_uint64):
        """
        Initializes the virtual machine with the specified size.

        Args:
            size: The size of the virtual machine.

        Returns:
            A ctypes.c_void_p representing a pointer to the initialized virtual machine.
        """
        self.ptr = self.vm_methods.init(size)

    def gc(self):
        """
        Performs garbage collection in the virtual machine.
        """
        return self.vm_methods.gc(self.ptr)

    def release(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Releases a resource managed by the virtual machine.

        Args:
            ptr: A ctypes.c_uint64 representing a pointer to the resource to release.

        Returns:
            A status code (typically 0 for success, non-zero for failure).
        """
        return self.vm_methods.release(self.ptr, ptr)

    def __del__(self):
        """
        Destroys the virtual machine.
        """
        return self.vm_methods.destroy(self.ptr)
