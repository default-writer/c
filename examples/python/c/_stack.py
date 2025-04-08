# c/stack.py
import ctypes
from ._vm import CVirtualMachine
from ._error import CException, CVirtualMachineNotInitializedException, CError
from ._pointer import CType


class CStack:
    """
    CStack class provides an interface to interact with stack operations in the C library.

    It allows allocating, pushing, peeking, popping, getting the size of, releasing, and freeing stacks.
    It relies on a CVirtualMachine instance for context.
    """

    class stack_methods(ctypes.Structure):
        """
        Structure representing the function pointers for stack operations in the C library.

        _fields_:
            alloc: Function pointer for allocating a stack.
            push: Function pointer for pushing an element onto the stack.
            peek: Function pointer for peeking at the top element of the stack.
            peekn: Function pointer for peeking at multiple elements from the top of the stack.
            pop: Function pointer for popping an element from the stack.
            popn: Function pointer for popping multiple elements from the stack.
            size: Function pointer for getting the size of the stack.
            release: Function pointer for releasing resources associated with the stack.
            free: Function pointer for freeing the stack.
        """
        _fields_ = [
            ("alloc", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p))),
            ("push", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("peek", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("peekn", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("pop", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("popn", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("size", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("release", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("free", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
        ]

    @classmethod
    def setup(cls, lib):
        """
        Sets up the CStack class by linking it to the C library's stack functions.

        Args:
            lib: The loaded C library object (e.g., from ctypes.CDLL).

        Raises:
            AttributeError: If the C library does not have a 'stack' attribute.
        """
        cls.stack_methods_ptr = lib.stack
        cls.stack_methods_ptr.restype = ctypes.POINTER(cls.stack_methods)
        cls.stack_methods = cls.stack_methods_ptr().contents

    def __init__(self, vm: CVirtualMachine):
        """
        Initializes a CStack instance.

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
    def alloc(self, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Allocates a new stack in the C library.

        Returns:
            A pointer (as a ctypes.c_uint64) to the newly allocated stack.

        Raises:
            CError: If an error occurs during the C library call.
        """
        return self.stack_methods.alloc(self.vm)

    @exception_handler
    def push(self, ptr: ctypes.c_uint64, data_ptr: ctypes.c_uint64, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Pushes an element onto the stack.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the stack.
            data_ptr: A pointer (as a ctypes.c_uint64) to the data to push.

        Returns:
            A status code (typically 0 for success, non-zero for failure).

        Raises:
            CError: If an error occurs during the C library call.
        """
        return self.stack_methods.push(self.vm, ptr, data_ptr)

    @exception_handler
    def peek(self, ptr: ctypes.c_uint64, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Peeks at the top element of the stack without removing it.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the stack.

        Returns:
            A pointer (as a ctypes.c_uint64) to the top element, or 0 if the stack is empty.

        Raises:
            CError: If an error occurs during the C library call.
        """
        return self.stack_methods.peek(self.vm, ptr)

    @exception_handler
    def peekn(self, ptr: ctypes.c_uint64, nelements: ctypes.c_uint64, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Peeks at multiple elements from the top of the stack without removing them.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the stack.
            nelements: The number of elements to peek.

        Returns:
            A pointer (as a ctypes.c_uint64) to a newly allocated stack containing the peeked elements,
            or 0 if an error occurs.

        Raises:
            CError: If an error occurs during the C library call.
        """
        return self.stack_methods.peekn(self.vm, ptr, nelements)

    @exception_handler
    def pop(self, ptr: ctypes.c_uint64, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Pops the top element from the stack.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the stack.

        Returns:
            A pointer (as a ctypes.c_uint64) to the popped element, or 0 if the stack is empty.

        Raises:
            CError: If an error occurs during the C library call.
        """
        return self.stack_methods.pop(self.vm, ptr)

    @exception_handler
    def popn(self, ptr: ctypes.c_uint64, nelements: ctypes.c_uint64, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Pops multiple elements from the stack.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the stack.
            nelements: The number of elements to pop.

        Returns:
            A pointer (as a ctypes.c_uint64) to a newly allocated stack containing the popped elements,
            or 0 if an error occurs.

        Raises:
            CError: If an error occurs during the C library call.
        """
        return self.stack_methods.popn(self.vm, ptr, nelements)

    @exception_handler
    def size(self, ptr: ctypes.c_uint64, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Gets the size of the stack.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the stack.

        Returns:
            The size of the stack.

        Raises:
            CError: If an error occurs during the C library call.
        """
        return self.stack_methods.size(self.vm, ptr)

    @exception_handler
    def release(self, ptr: ctypes.c_uint64, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Releases resources associated with the stack.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the stack.

        Returns:
            A status code (typically 0 for success, non-zero for failure).

        Raises:
            CError: If an error occurs during the C library call.
        """
        return self.stack_methods.release(self.vm, ptr)

    @exception_handler
    def free(self, ptr: ctypes.c_uint64, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Frees the stack.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the stack.

        Returns:
            A status code (typically 0 for success, non-zero for failure).

        Raises:
            CError: If an error occurs during the C library call.
        """
        return self.stack_methods.free(self.vm, ptr)
