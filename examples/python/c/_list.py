# examples/python/c/_list.py
import ctypes
from ._vm import CVirtualMachine
from ._error import CError, CException, CVirtualMachineNotInitializedException


class CList:
    """
    CList class provides an interface to interact with list-related functions
    in a C library using a virtual machine context.

    It allows initializing, destroying, pushing, popping, peeking, and comparing lists,
    mirroring the functionality defined in list_v1.h. It relies on a
    CVirtualMachine instance for context. Error handling for C library calls
    is managed by the exception_handler decorator.

    """

    class list_methods(ctypes.Structure):
        """
        Structure representing the function pointers for list operations in the C library.

        _fields_:
            init: Function pointer for initializing a list (takes vm_ptr, returns stack_ptr).
            push: Function pointer for pushing an element onto a list (takes vm_ptr, stack_ptr, item (void_ptr interpreted as u64), returns u64 status).
            pop: Function pointer for popping an element from a list (takes vm_ptr, stack_ptr, returns item (void_ptr interpreted as u64)).
            peek: Function pointer for peeking at the top element of a list (takes vm_ptr, stack_ptr, returns item (void_ptr interpreted as u64)).
            diff: Function pointer for computing symmetric difference (takes vm_ptr, stack_ptr1, stack_ptr2, result_stack_ptr, returns u64 status).
            diff_left: Function pointer for computing left difference (takes vm_ptr, stack_ptr1, stack_ptr2, result_stack_ptr, returns u64 status).
            diff_right: Function pointer for computing right difference (takes vm_ptr, stack_ptr1, stack_ptr2, result_stack_ptr, returns u64 status).
            destroy: Function pointer for destroying a list (takes vm_ptr, stack_ptr, returns u64 status).
        """
        _fields_ = [
            ("init", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p))),
            ("push", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("pop", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p))),
            ("peek", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p))),
            ("diff", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p))),
            ("diff_left", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p))),
            ("diff_right", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p))),
            ("destroy", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p)))
        ]

    @classmethod
    def setup(cls, lib):
        """
        Sets up the CList class by linking it to the C library's list functions.

        Args:
            lib: The loaded C library object (e.g., from ctypes.CDLL).

        """
        cls.list_methods_ptr = lib.list
        cls.list_methods_ptr.restype = ctypes.POINTER(cls.list_methods)
        cls.list_methods = cls.list_methods_ptr().contents

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

    def __init__(self, vm: CVirtualMachine):
        """
        Initializes a CList instance.

        Args:
            vm: An instance of CVirtualMachine, representing the virtual machine context.

        Raises:
            CVirtualMachineNotInitializedException: If the provided VM is None or its pointer is NULL.
        """
        if vm is None or not vm.ptr:
            raise CVirtualMachineNotInitializedException(f"VM is not initialized for this {self.__class__.__name__} instance.")
        self.vm = vm.ptr

    def __enter__(self):
        """
        Allows the CList to be used as a context manager.

        Returns:
            The CList instance.
        """
        self.ptr = self.init()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        """
        Cleans up the list when exiting the context.
        """
        self.destroy()

    @exception_handler
    def init(self, nothrow=False, noclear=False) -> ctypes.c_void_p:
        """
        Initializes a new list in the C library associated with the VM context.

        Args:
            nothrow (bool): If True, suppress Python exception on C error.
            noclear (bool): If True, do not clear C error before the call.

        Returns:
            A pointer to the newly initialized list.
        """
        return self.list_methods.init(self.vm)

    @exception_handler
    def destroy(self, nothrow=False, noclear=False) -> int:
        """
        Destroys a list previously created in the C library.

        Args:
            list_ptr: A pointer to the list to destroy.

        Returns:
            An integer representing the u64 status code from the C function.
        """
        return self.list_methods.destroy(self.vm, self.ptr)

    @exception_handler
    def push(self, data: int, nothrow=False, noclear=False) -> int:
        """
        Pushes an element (data) onto the specified list in the C library.

        Args:
            list_ptr: A pointer to the list.
            data: A Python integer representing the u64 data to push onto the list.

        Returns:
            An integer representing the u64 status code from the C function.
        """
        return self.list_methods.push(self.vm, self.ptr, data)

    @exception_handler
    def pop(self, nothrow=False, noclear=False) -> int:
        """
        Pops an element from the specified list in the C library.

        Args:
            list_ptr: A pointer to the list.

        Returns:
            A Python integer representing the u64 data popped from the list.
        """
        return self.list_methods.pop(self.vm, self.ptr)

    @exception_handler
    def peek(self, nothrow=False, noclear=False) -> int:
        """
        Peeks at the top element of the specified list without removing it.

        Args:
            list_ptr: A pointer to the list.

        Returns:
            A Python integer representing the u64 data at the top of the list.
        """
        return self.list_methods.peek(self.vm, self.ptr)

    @exception_handler
    def diff(self, stack1: ctypes.c_void_p, stack2: ctypes.c_void_p, nothrow=False, noclear=False) -> int:
        """
        Computes the symmetric difference between two lists and stores it in a third list.

        Args:
            stack1: A pointer to the first list.
            stack2: A pointer to the second list.

        Returns:
            An integer representing the u64 status code from the C function.
        """
        return self.list_methods.diff(self.vm, stack1, stack2, self.ptr)

    @exception_handler
    def diff_left(self, stack1: ctypes.c_void_p, stack2: ctypes.c_void_p, nothrow=False, noclear=False) -> int:
        """
        Computes the elements present in the first list but not in the second (left difference).

        Args:
            stack1: A pointer to the first list.
            stack2: A pointer to the second list.

        Returns:
            An integer representing the u64 status code from the C function.
        """
        return self.list_methods.diff_left(self.vm, stack1, stack2, self.ptr)

    @exception_handler
    def diff_right(self, stack1: ctypes.c_void_p, stack2: ctypes.c_void_p, nothrow=False, noclear=False) -> int:
        """
        Computes the elements present in the second list but not in the first (right difference).

        Args:
            stack1: A pointer to the first list.
            stack2: A pointer to the second list.

        Returns:
            An integer representing the u64 status code from the C function.
        """
        return self.list_methods.diff_right(self.vm, stack1, stack2, self.ptr)
