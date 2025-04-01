# c/pointer.py
import ctypes


class CType(ctypes.c_uint64):
    """
    CType is an enumeration of the different data types that can be managed by the C library.

    It is used to identify the type of data associated with a pointer.
    """
    Default = 0
    """Represents a default or unknown data type."""
    Data = 1
    """Represents a generic data block."""
    String = 2
    """Represents a null-terminated string."""
    StringPointer = 3
    """Represents a pointer to a null-terminated string."""
    File = 4
    """Represents a file."""
    Stack = 5
    """Represents a stack data structure."""
    Object = 6
    """Represents a generic object."""
    User = 7
    """Represents a user-defined data type."""


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
            ("read", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("free", ctypes.CFUNCTYPE(None, ctypes.c_uint64, ctypes.c_uint64)),
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

    @classmethod
    def alloc(cls, data: ctypes.c_void_p, size: ctypes.c_uint64, type_id: CType) -> ctypes.c_uint64:
        """
        Allocates a pointer in the C library.

        Args:
            data: A ctypes.c_void_p representing a pointer to the data to be associated with the new pointer.
            size: The size of the data pointed to.
            type_id: A CType value representing the type of data.

        Returns:
            A ctypes.c_uint64 representing the address of the newly allocated pointer.
        """
        return cls.pointer_methods.alloc(data, size, type_id)

    @classmethod
    def copy(cls, src: ctypes.c_void_p, size: ctypes.c_uint64, offset: ctypes.c_uint64, type_id: CType) -> ctypes.c_uint64:
        """
        Copies a pointer in the C library.

        Args:
            src: A ctypes.c_void_p representing a pointer to the source data.
            size: The size of the data to copy.
            offset: The offset within the source data to start copying from.
            type_id: A CType value representing the type of data.

        Returns:
            A ctypes.c_uint64 representing the address of the newly copied pointer.
        """
        return cls.pointer_methods.copy(src, size, offset, type_id)

    @classmethod
    def read(cls, address: ctypes.c_uint64, type_id: CType) -> ctypes.c_void_p:
        """
        Reads data from a pointer in the C library.

        Args:
            address: A ctypes.c_uint64 representing the address of the pointer to read from.
            type_id: A CType value representing the type of data.

        Returns:
            A ctypes.c_void_p representing a pointer to the data read.
        """
        return cls.pointer_methods.read(address, type_id)

    @classmethod
    def free(cls, address: ctypes.c_uint64, type_id: CType) -> ctypes.c_uint64:
        """
        Frees a pointer in the C library.

        Args:
            address: A ctypes.c_uint64 representing the address of the pointer to free.
            type_id: A CType value representing the type of data.

        Returns:
            A status code (typically 0 for success, non-zero for failure).
        """
        return cls.pointer_methods.free(address, type_id)
