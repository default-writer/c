# c/file.py
import ctypes
from ._vm import CVirtualMachine
from ._error import CError, CException, CVirtualMachineNotInitializedException
from ._pointer import CType


class CFile:
    """
    CFile class provides an interface to interact with file-related operations
    in a C library.

    It allows allocating a file, accessing file data, and freeing file resources.
    It relies on a CVirtualMachine instance for context.
    """

    class file_methods(ctypes.Structure):
        """
        Structure representing the function pointers for file operations in the C library.

        _fields_:
            alloc: Function pointer for allocating a file.
            data: Function pointer for getting the data of a file.
            free: Function pointer for freeing a file.
        """
        _fields_ = [
            ("alloc", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("data", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("free", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
        ]

    @classmethod
    def setup(cls, lib):
        """
        Sets up the CFile class by linking it to the C library's file functions.

        Args:
            lib: The loaded C library object (e.g., from ctypes.CDLL).

        Raises:
            AttributeError: If the C library does not have a 'file' attribute.
        """
        cls.file_methods_ptr = lib.file
        cls.file_methods_ptr.restype = ctypes.POINTER(cls.file_methods)
        cls.file_methods = cls.file_methods_ptr().contents

    def __init__(self, vm: CVirtualMachine):
        """
        Initializes a CFile instance.

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
            if not kwargs.get("noclear", False):
                CError.clear()
            result = func(self, *args, **kwargs)
            if not kwargs.get("nothrow", False):
                CException.check()
            return result
        return wrapper

    @exception_handler
    def alloc(self, file_path_ptr: ctypes.c_uint64, mode_ptr: ctypes.c_uint64, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Allocates a file in the C library.

        Args:
            file_path_ptr: A pointer (as a ctypes.c_uint64) to a null-terminated string representing the file path.
            mode_ptr: A pointer (as a ctypes.c_uint64) to a null-terminated string representing the file mode (e.g., "r", "w").

        Returns:
            A pointer (as a ctypes.c_uint64) to the allocated file.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.file_methods.alloc(self.vm, file_path_ptr, mode_ptr)

    @exception_handler
    def data(self, ptr: ctypes.c_uint64, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Gets the data associated with a file.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the allocated file.

        Returns:
            A pointer (as a ctypes.c_uint64) to the file data.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.file_methods.data(self.vm, ptr)

    @exception_handler
    def free(self, ptr: ctypes.c_uint64, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Frees the resources associated with a file.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the allocated file.

        Returns:
            A status code (typically 0 for success, non-zero for failure).

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.file_methods.free(self.vm, ptr)
