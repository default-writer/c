# c/env.py
import ctypes
from ._vm import CVirtualMachine
from ._error import CError, CException, CVirtualMachineNotInitializedException


class CEnvironment:
    """
    CEnvironment class provides an interface to interact with environment-related functions
    in a C library.

    It allows retrieving environment variables, getting the current working directory,
    and printing strings to the standard output. It relies on a CVirtualMachine instance
    for context.
    """

    class env_methods(ctypes.Structure):
        """
        Structure representing the function pointers for environment operations in the C library.

        _fields_:
            getenv: Function pointer for retrieving an environment variable.
            getcwd: Function pointer for getting the current working directory.
            puts: Function pointer for printing a string to the standard output.
        """
        _fields_ = [
            ("getenv", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("getcwd", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p))),
            ("puts", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
        ]

    @classmethod
    def setup(cls, lib):
        """
        Sets up the CEnvironment class by linking it to the C library's environment functions.

        Args:
            lib: The loaded C library object (e.g., from ctypes.CDLL).

        Raises:
            AttributeError: If the C library does not have an 'env' attribute.
        """
        cls.env_methods_ptr = lib.env
        cls.env_methods_ptr.restype = ctypes.POINTER(cls.env_methods)
        cls.env_methods = cls.env_methods_ptr().contents

    def __init__(self, vm: CVirtualMachine):
        """
        Initializes a CEnvironment instance.

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
    def getenv(self, name_ptr: ctypes.c_uint64, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Retrieves the value of an environment variable.

        Args:
            name_ptr: A pointer (as a ctypes.c_uint64) to a null-terminated string representing the environment variable name.

        Returns:
            A pointer (as a ctypes.c_uint64) to a null-terminated string representing the environment variable value,
            or 0 if the variable is not found.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.env_methods.getenv(self.vm, name_ptr)

    @exception_handler
    def getcwd(self, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Gets the current working directory.

        Returns:
            A pointer (as a ctypes.c_uint64) to a null-terminated string representing the current working directory.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.env_methods.getcwd(self.vm)

    @exception_handler
    def puts(self, ptr: ctypes.c_uint64, nothrow=False, noclear=False) -> ctypes.c_uint64:
        """
        Prints a null-terminated string to the standard output.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to a null-terminated string to be printed.

        Returns:
            A status code (typically 0 for success, non-zero for failure).

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.env_methods.puts(self.vm, ptr)
