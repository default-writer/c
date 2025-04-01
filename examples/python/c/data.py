# c/data.py
import ctypes
from .vm import CVirtualMachine
from .error import CException, CVirtualMachineNotInitializedException


class CData:
    """
    CData class provides an interface to interact with a C library's data management functions.

    It allows allocating, querying the size of, accessing raw data, and freeing memory blocks
    managed by the C library. It relies on a CVirtualMachine instance for context.
    """

    class data_methods(ctypes.Structure):
        """
        Structure representing the function pointers for data operations in the C library.

        _fields_:
            alloc: Function pointer for allocating memory.
            size: Function pointer for getting the size of allocated memory.
            unsafe: Function pointer for getting a raw pointer to the allocated memory.
            free: Function pointer for freeing allocated memory.
        """
        _fields_ = [
            ("alloc", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("size", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("unsafe", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("free", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
        ]

    @classmethod
    def setup(cls, lib):
        """
        Sets up the CData class by linking it to the C library's data management functions.

        Args:
            lib: The loaded C library object (e.g., from ctypes.CDLL).

        Raises:
            AttributeError: If the C library does not have a 'data' attribute.
        """
        cls.data_methods_ptr = lib.data
        cls.data_methods_ptr.restype = ctypes.POINTER(cls.data_methods)
        cls.data_methods = cls.data_methods_ptr().contents

    def __init__(self, vm: CVirtualMachine):
        """
        Initializes a CData instance.

        Args:
            vm: An instance of CVirtualMachine, representing the virtual machine context.

        Raises:
            CVirtualMachineNotInitializedException: If the provided VM is None.
        """
        if vm is None:
            raise CVirtualMachineNotInitializedException(f"VM is not initialized for this {self.__class__.__name__} instance.")
        self.vm = vm.ptr

    def exception_handler(func):
        """
        Decorator for handling exceptions raised by C library calls.

        It checks for errors after each C function call and raises a Python exception if an error occurred.

        Args:
            func: The function to be wrapped.

        Returns:
            The wrapped function.
        """
        def wrapper(self, *args, **kwargs):
            result = func(self, *args, **kwargs)
            CException.check()
            return result
        return wrapper

    @exception_handler
    def alloc(self, size: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Allocates a memory block of the specified size in the C library.

        Args:
            size: The size of the memory block to allocate.

        Returns:
            A pointer (as a ctypes.c_uint64) to the allocated memory block.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.data_methods.alloc(self.vm, size)

    @exception_handler
    def size(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Gets the size of the memory block pointed to by the given pointer.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the allocated memory block.

        Returns:
            The size of the memory block.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.data_methods.size(self.vm, ptr)

    @exception_handler
    def unsafe(self, ptr: ctypes.c_uint64) -> ctypes.c_void_p:
        """
        Gets an unsafe pointer to the allocated memory block.

        This pointer can be used to directly access the raw data in the memory block.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the allocated memory block.

        Returns:
            A ctypes.c_void_p representing the raw pointer to the memory block.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.data_methods.unsafe(self.vm, ptr)

    @exception_handler
    def free(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Frees the memory block pointed to by the given pointer.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the allocated memory block.

        Returns:
            A status code (typically 0 for success, non-zero for failure).

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.data_methods.free(self.vm, ptr)
