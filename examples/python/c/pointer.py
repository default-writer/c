# c/pointer.py
import ctypes
from .vm import CVirtualMachine
from .error import CException, CVirtualMachineNotInitializedException


class CType(ctypes.c_uint64):
    """
    CType is an enumeration of the different data types that can be managed by the C library.

    It is used to identify the type of data associated with a pointer.
    These values are directly mapped to the `enum type` in `std/data.h`.
    """
    TYPE_NULL = 0
    """Represents a null or ephemeral data type (TYPE_NULL)."""
    TYPE_DATA = 1
    """Represents a generic data block (TYPE_DATA)."""
    TYPE_FILE = 2
    """Represents a file (TYPE_FILE)."""
    TYPE_OBJECT = 3
    """Represents a generic object (TYPE_OBJECT)."""
    TYPE_STACK = 4
    """Represents a stack data structure (TYPE_STACK)."""
    TYPE_STRING = 5
    """Represents a null-terminated string (TYPE_STRING)."""
    TYPE_STRING_POINTER = 6
    """Represents a pointer to a null-terminated string (TYPE_STRING_POINTER)."""
    TYPE_USER = 7
    """Represents a user-defined data type (TYPE_USER)."""


class CPointer:
    """
    CPointer class provides an interface to interact with pointer management functions
    in a C library.

    It allows allocating, copying, reading, and freeing pointers, associating them with
    specific data types.
    """

    class pointer_methods(ctypes.Structure):
        """
        Structure representing the function pointers for pointer operations in the C library.

        _fields_:
            alloc: Function pointer for allocating a pointer.
            copy: Function pointer for copying a pointer.
            read: Function pointer for reading data from a pointer.
            free: Function pointer for freeing a pointer.
        """
        _fields_ = [
            ("alloc", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("copy", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64, ctypes.c_uint64)),
            ("read", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("free", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
        ]

    @classmethod
    def setup(cls, lib):
        """
        Sets up the CPointer class by linking it to the C library's pointer functions.

        Args:
            lib: The loaded C library object (e.g., from ctypes.CDLL).

        Raises:
            AttributeError: If the C library does not have a 'pointer' attribute.
        """
        cls.pointer_methods_ptr = lib.pointer
        cls.pointer_methods_ptr.restype = ctypes.POINTER(cls.pointer_methods)
        cls.pointer_methods = cls.pointer_methods_ptr().contents

    def __init__(self, vm: CVirtualMachine):
        """
        Initializes a CPointer instance.

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
    def alloc(self, data: ctypes.c_uint64, size: ctypes.c_uint64, type_id: CType) -> ctypes.c_uint64:
        """
        Allocates a pointer in the C library.

        Args:
            data: A ctypes.c_uint64 representing a pointer to the data to be associated with the new pointer.
            size: The size of the data pointed to.
            type_id: A CType value representing the type of data.

        Returns:
            A ctypes.c_uint64 representing the address of the newly allocated pointer.
        """
        return self.pointer_methods.alloc(self.vm, data, size)

    @exception_handler
    def copy(self, src: ctypes.c_uint64, size: ctypes.c_uint64, offset: ctypes.c_uint64, type_id: CType) -> ctypes.c_uint64:
        """
        Copies a pointer in the C library.

        Args:
            src: A ctypes.c_uint64 representing a pointer to the source data.
            size: The size of the data to copy.
            offset: The offset within the source data to start copying from.
            type_id: A CType value representing the type of data.

        Returns:
            A ctypes.c_uint64 representing the address of the newly copied pointer.
        """
        return self.pointer_methods.copy(self.vm, src, size, offset, type_id)

    @exception_handler
    def read(self, address: ctypes.c_uint64, type_id: CType) -> ctypes.c_void_p:
        """
        Reads data from a pointer in the C library.

        Args:
            address: A ctypes.c_uint64 representing the address of the pointer to read from.
            type_id: A CType value representing the type of data.

        Returns:
            A ctypes.c_void_p representing a pointer to the data read.
        """
        return self.pointer_methods.read(self.vm, address, type_id)

    @exception_handler
    def free(self, address: ctypes.c_uint64, type_id: CType) -> ctypes.c_uint64:
        """
        Frees a pointer in the C library.

        Args:
            address: A ctypes.c_uint64 representing the address of the pointer to free.
            type_id: A CType value representing the type of data.

        Returns:
            A status code (typically 0 for success, non-zero for failure).
        """
        return self.pointer_methods.free(self.vm, address, type_id)
