# c/error.py
import ctypes


class CErrorType:
    """
    CErrorType defines an enumeration of error types that can occur within the C library.

    These error types are used to categorize and identify specific issues encountered
    during the execution of C functions.
    """
    Default = 0
    """Represents no error or a default state."""
    VirtualMachineNotInitializedError = 1
    """Indicates that the virtual machine has not been properly initialized."""
    InvalidPointerError = 2
    """Indicates that an invalid memory pointer was encountered."""
    InvalidArgumentError = 3
    """Indicates that an invalid argument was passed to a function."""
    InvalidTypeIdError = 4
    """Indicates that an invalid type identifier was encountered."""
    InvalidValueError = 5
    """Indicates that an invalid value was encountered."""


class CError:
    """
    CError provides an interface to interact with the C library's error handling functions.

    It allows setting up the error handling mechanism, accessing standard output and
    standard error streams, throwing errors, clearing errors, and retrieving error
    information.
    """
    class error_methods(ctypes.Structure):
        """
        Structure representing the function pointers for error operations in the C library.

        _fields_:
            stdout: Function pointer for getting the standard output stream.
            stderr: Function pointer for getting the standard error stream.
            output: Function pointer for outputting an error message.
            throw: Function pointer for throwing an error.
            clear: Function pointer for clearing the current error.
            type: Function pointer for getting the type of the current error.
            get: Function pointer for getting the message of the current error.
        """
        _fields_ = [
            ("stdout", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p))),
            ("stderr", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p))),
            ("output", ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_char_p, ctypes.c_uint64)),
            ("throw", ctypes.CFUNCTYPE(None, ctypes.c_uint64, ctypes.c_char_p, ctypes.c_uint64)),
            ("clear", ctypes.CFUNCTYPE(None)),
            ("type", ctypes.CFUNCTYPE(ctypes.c_uint64)),
            ("get", ctypes.CFUNCTYPE(ctypes.c_char_p)),
        ]

    @classmethod
    def setup(cls, lib):
        """
        Sets up the CError class by linking it to the C library's error handling functions.

        Args:
            lib: The loaded C library object (e.g., from ctypes.CDLL).
        """
        cls.error_methods_ptr = lib.error
        cls.error_methods_ptr.restype = ctypes.POINTER(cls.error_methods)
        cls.error_methods = cls.error_methods_ptr().contents

    @classmethod
    def stdout(cls) -> ctypes.c_void_p:
        """
        Gets the standard output stream from the C library.

        Returns:
            A pointer to the standard output stream.
        """
        return cls.error_methods.stdout()

    @classmethod
    def stderr(cls) -> ctypes.c_void_p:
        """
        Gets the standard error stream from the C library.

        Returns:
            A pointer to the standard error stream.
        """
        return cls.error_methods.stderr()

    @classmethod
    def output(cls, output_file: ctypes.c_void_p, code: CErrorType, message: ctypes.c_char_p, size: ctypes.c_uint64):
        """
        Outputs an error message to a specified output file.

        Args:
            output_file: A pointer to the output file.
            code: The error code.
            message: The error message as a C-style string.
            size: The size of the error message.
        """
        cls.error_methods.output(output_file, code, message, size)

    @classmethod
    def throw(cls, code: CErrorType, message: ctypes.c_char_p, size: ctypes.c_uint64):
        """
        Throws an error with the specified code and message.

        Args:
            code: The error code.
            message: The error message as a C-style string.
            size: The size of the error message.
        """
        cls.error_methods.throw(code, message, size)

    @classmethod
    def clear(cls):
        """
        Clears the current error in the C library.
        """
        cls.error_methods.clear()

    @classmethod
    def type(cls) -> CErrorType:
        """
        Gets the type of the current error in the C library.

        Returns:
            The error type.
        """
        return cls.error_methods.type()

    @classmethod
    def get(cls) -> ctypes.c_char_p:
        """
        Gets the message of the current error in the C library.

        Returns:
            The error message as a C-style string.
        """
        return cls.error_methods.get()


class CException(Exception):
    """
    CException provides a mechanism for checking for errors in the C library and
    raising corresponding Python exceptions.

    It maintains the last error code and message, and provides methods for checking
    for errors and raising exceptions.
    """
    last_error_code: CErrorType = CErrorType.Default
    """The last error code encountered."""
    last_error_message: str = ""
    """The last error message encountered."""

    @classmethod
    def check(cls):
        """
        Checks for errors in the C library and updates the last error code and message.

        If an error is found, it calls raise_exception to raise a corresponding
        Python exception.
        """
        cls.last_error_code = CError.type()
        if cls.last_error_code:
            cls.last_error_message = CError.get().decode()
            cls.raise_exception()

    @classmethod
    def raise_exception(cls):
        """
        Raises a Python exception corresponding to the last error code.

        It clears the error in the C library after raising the exception.

        Raises:
            Exception: A Python exception corresponding to the last error code.
        """
        raise cls.get_exception()

    @classmethod
    def get_exception(cls):
        """
        Gets the appropriate Python exception based on the last error code.

        Returns:
            Exception: A Python exception corresponding to the last error code.

        Raises:
            Exception: If an unknown error code is encountered.
        """
        if cls.last_error_code == CErrorType.VirtualMachineNotInitializedError:
            return CVirtualMachineNotInitializedException(cls.last_error_message)
        elif cls.last_error_code == CErrorType.InvalidPointerError:
            return CInvalidPointerException(cls.last_error_message)
        elif cls.last_error_code == CErrorType.InvalidArgumentError:
            return CInvalidArgumentException(cls.last_error_message)
        elif cls.last_error_code == CErrorType.InvalidTypeIdError:
            return CInvalidTypeIdException(cls.last_error_message)
        elif cls.last_error_code == CErrorType.InvalidValueError:
            return CInvalidValueException(cls.last_error_message)
        else:
            return Exception(f"Unknown error code: {cls.last_error_code}, message: {cls.last_error_message}")


class CVirtualMachineNotInitializedException(CException):
    """
    CVirtualMachineNotInitializedException is raised when an operation is attempted
    that requires a properly initialized virtual machine, but the VM has not been
    initialized.
    """
    def __init__(self, message):
        """
        Initializes the exception with a message.

        Args:
            message: The error message.
        """
        super().__init__(message)


class CInvalidPointerException(CException):
    """
    CInvalidPointerException is raised when an operation is attempted with an
    invalid memory pointer.
    """
    def __init__(self, message):
        """
        Initializes the exception with a message.

        Args:
            message: The error message.
        """
        super().__init__(message)


class CInvalidArgumentException(CException):
    """
    CInvalidArgumentException is raised when an operation is attempted with an
    invalid argument.
    """
    def __init__(self, message):
        """
        Initializes the exception with a message.

        Args:
            message: The error message.
        """
        super().__init__(message)


class CInvalidTypeIdException(CException):
    """
    CInvalidTypeIdException is raised when an operation is attempted with an
    invalid type identifier.
    """
    def __init__(self, message):
        """
        Initializes the exception with a message.

        Args:
            message: The error message.
        """
        super().__init__(message)


class CInvalidValueException(CException):
    """
    CInvalidValueException is raised when an operation is attempted with an
    invalid value.
    """
    def __init__(self, message):
        """
        Initializes the exception with a message.

        Args:
            message: The error message.
        """
        super().__init__(message)
