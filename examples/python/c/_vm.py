# c/vm.py
import ctypes
from ._error import CError, CException, CVirtualMachineNotInitializedException


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
            dump_ref: Function pointer for dumping memory references.
            dump_ref_stack: Function pointer for dumping memory references from a stack.
        """
        _fields_ = [
            ("init", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("gc", ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_void_p))),
            ("release", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("destroy", ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_void_p))),
            ("dump_ref", ctypes.CFUNCTYPE(None, ctypes.c_void_p)),
            ("dump_ref_stack", ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.POINTER(ctypes.c_void_p))),
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

    def exception_handler(func):
        """
        Decorator for handling exceptions raised by C library calls.

        It checks for errors using CError/CException after each C function call
        and raises a Python exception if an error occurred.

        Args:
            func: The function to be wrapped.

        Returns:
            The wrapped function.
        """
        def wrapper(self, *args, **kwargs):
            if not kwargs.get("noclear", False):
                CError.clear()
            result = func(self, *args, **kwargs)
            if not kwargs.get("nothrow", False):
                CException.check()
            return result
        return wrapper

    def __init__(self, size: ctypes.c_uint64):
        """
        Initializes the virtual machine with the specified size.

        Args:
            size: The size of the virtual machine.

        """
        self.size = size

    def __enter__(self):
        """
        Allows the CVirtualMachine to be used as a context manager.

        Returns:
            The CVirtualMachine instance.
        """
        self.ptr = self.init()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        """
        Cleans up the virtual machine when exiting the context.
        """
        self.gc()
        self.destroy()

    @exception_handler
    def init(self, nothrow=False, noclear=False) -> ctypes.c_void_p:
        """
        Initializes a new VM context.

        Args:
            nothrow (bool): If True, suppress Python exception on C error.
            noclear (bool): If True, do not clear C error before the call.

        Returns:
            A pointer to the newly initialized VM.
        """
        return self.vm_methods.init(self.size)

    @exception_handler
    def gc(self):
        """
        Performs garbage collection in the virtual machine.
        """
        return self.vm_methods.gc(self.ptr)

    @exception_handler
    def release(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Releases a resource managed by the virtual machine.

        Args:
            ptr: A pointer to the resource to release.

        Returns:
            A status code (typically 0 for success, non-zero for failure).
        """
        return self.vm_methods.release(self.ptr, ptr)

    @exception_handler
    def destroy(self):
        """
        Cleans up the virtual machine when exiting the context.
        """
        self.vm_methods.destroy(self.ptr)

    @exception_handler
    def dump_ref(self):
        """
        Dumps memory references.
        """
        self.vm_methods.dump_ref(self.ptr)

    @exception_handler
    def dump_ref_stack(self, stack_ptr: ctypes.c_void_p):
        """
        Dumps memory references to a stack.

        Args:
            stack: The stack to dump references to.
        """
        self.vm_methods.dump_ref_stack(self.ptr, stack_ptr)
