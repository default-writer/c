# c/memory.py
import ctypes


class CMemory:
    """
    CMemory class provides an interface to interact with raw memory management functions
    in a C library.

    It allows allocating, reallocating, and freeing raw memory blocks.
    It does not rely on a CVirtualMachine instance.
    """

    class memory_methods(ctypes.Structure):
        """
        Structure representing the function pointers for memory operations in the C library.

        _fields_:
            alloc: Function pointer for allocating memory.
            realloc: Function pointer for reallocating memory.
            free: Function pointer for freeing memory.
        """
        _fields_ = [
            ("alloc", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.c_size_t, ctypes.c_size_t)),
            ("realloc", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.c_size_t)),
            ("free", ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_void_p))),
        ]

    @classmethod
    def setup(cls, lib):
        """
        Sets up the CMemory class by linking it to the C library's memory functions.

        Args:
            lib: The loaded C library object (e.g., from ctypes.CDLL).

        Raises:
            AttributeError: If the C library does not have a 'memory' attribute.
        """
        cls.memory_methods_ptr = lib.memory
        cls.memory_methods_ptr.restype = ctypes.POINTER(cls.memory_methods)
        cls.memory_methods = cls.memory_methods_ptr().contents

    @classmethod
    def alloc(cls, nmemb: ctypes.c_size_t, size: ctypes.c_size_t) -> ctypes.c_void_p:
        """
        Allocates a raw memory block of the specified size in the C library.

        Args:
            nmemb: The number of elements to allocate.
            size: The size of each element in bytes.

        Returns:
            A ctypes.c_void_p representing a pointer to the allocated memory block.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return cls.memory_methods.alloc(nmemb, size)

    @classmethod
    def realloc(cls, ptr: ctypes.c_void_p, size: ctypes.c_size_t) -> ctypes.c_void_p:
        """
        Reallocates a raw memory block to a new size in the C library.

        Args:
            ptr: A ctypes.c_void_p representing a pointer to the previously allocated memory block.
            size: The new size to reallocate the memory block to.

        Returns:
            A ctypes.c_void_p representing a pointer to the reallocated memory block.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return cls.memory_methods.realloc(ptr, size)

    @classmethod
    def free(cls, ptr: ctypes.c_void_p):
        """
        Frees a raw memory block previously allocated in the C library.

        Args:
            ptr: A ctypes.c_void_p representing a pointer to the memory block to free.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return cls.memory_methods.free(ptr)
