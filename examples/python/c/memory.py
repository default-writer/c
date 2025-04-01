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
            ("alloc", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("realloc", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("free", ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
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
    def alloc(cls, size: ctypes.c_uint64) -> ctypes.c_void_p:
        """
        Allocates a raw memory block of the specified size in the C library.

        Args:
            size: The size of the memory block to allocate.

        Returns:
            A ctypes.c_void_p representing a pointer to the allocated memory block.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return cls.memory_methods.alloc(size)

    @classmethod
    def realloc(cls, ptr: ctypes.c_void_p, size: ctypes.c_uint64, new_size: ctypes.c_uint64) -> ctypes.c_void_p:
        """
        Reallocates a raw memory block to a new size in the C library.

        Args:
            ptr: A ctypes.c_void_p representing a pointer to the previously allocated memory block.
            size: The current size of the memory block.
            new_size: The new size to reallocate the memory block to.

        Returns:
            A ctypes.c_void_p representing a pointer to the reallocated memory block.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return cls.memory_methods.realloc(ptr, size, new_size)

    @classmethod
    def free(cls, ptr: ctypes.c_void_p, size: ctypes.c_uint64):
        """
        Frees a raw memory block previously allocated in the C library.

        Args:
            ptr: A ctypes.c_void_p representing a pointer to the memory block to free.
            size: The size of the memory block to free.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return cls.memory_methods.free(ptr, size)
