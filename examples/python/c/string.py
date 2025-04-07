# c/string.py
import ctypes
from .vm import CVirtualMachine
from .error import CException, CVirtualMachineNotInitializedException


class CString:
    """
    CString class provides an interface to interact with string manipulation functions
    in a C library.

    It allows performing operations such as allocating, copying, concatenating,
    searching, comparing, and modifying strings. It relies on a CVirtualMachine
    instance for context.
    """

    class string_methods(ctypes.Structure):
        """
        Structure representing the function pointers for string operations in the C library.

        _fields_:
            free: Function pointer for freeing a string.
            copy: Function pointer for copying a string.
            strcpy: Function pointer for copying a string to another string.
            strcat: Function pointer for concatenating two strings.
            strrchr: Function pointer for finding the last occurrence of a character in a string.
            strchr: Function pointer for finding the first occurrence of a character in a string.
            match: Function pointer for checking if a string matches another string.
            offset: Function pointer for getting the offset of a substring in a string.
            load: Function pointer for loading a string from a C-style string.
            put_char: Function pointer for putting a character into a string.
            unsafe: Function pointer for getting an unsafe pointer to the string data.
            size: Function pointer for getting the size of a string.
            lessthan: Function pointer for checking if one string is less than another.
            greaterthan: Function pointer for checking if one string is greater than another.
            equals: Function pointer for checking if two strings are equal.
            compare: Function pointer for comparing two strings.
            left: Function pointer for getting the left part of a string.
            strncpy: Function pointer for copying a part of a string.
            left_strncpy: Function pointer for copying a part of a string from the left.
            right: Function pointer for getting the right part of a string.
            move_left: Function pointer for moving a part of a string to the left.
            move_right: Function pointer for moving a part of a string to the right.
            strcmp: Function pointer for comparing two strings.
        """
        _fields_ = [
            ("free", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("copy", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("strcpy", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("strcat", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("strrchr", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("strchr", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("match", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("offset", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("load", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_char_p)),
            ("put_char", ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_char)),
            ("unsafe", ctypes.CFUNCTYPE(ctypes.c_char_p, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("size", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("lessthan", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("greaterthan", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("equals", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("compare", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("left", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("strncpy", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("left_strncpy", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("right", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("move_left", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("move_right", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("strcmp", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
        ]

    @classmethod
    def setup(cls, lib):
        """
        Sets up the CString class by linking it to the C library's string functions.

        Args:
            lib: The loaded C library object (e.g., from ctypes.CDLL).

        Raises:
            AttributeError: If the C library does not have a 'string' attribute.
        """
        cls.string_methods_ptr = lib.string
        cls.string_methods_ptr.restype = ctypes.POINTER(cls.string_methods)
        cls.string_methods = cls.string_methods_ptr().contents

    def __init__(self, vm: CVirtualMachine):
        """
        Initializes a CString instance.

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
    def free(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Frees a string in the C library.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the string to free.

        Returns:
            A status code (typically 0 for success, non-zero for failure).

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.free(self.vm, ptr)

    @exception_handler
    def copy(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Copies a string in the C library.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the string to copy.

        Returns:
            A pointer (as a ctypes.c_uint64) to the newly copied string.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.copy(self.vm, ptr)

    @exception_handler
    def strcpy(self, dest: ctypes.c_uint64, src: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Copies a string to another string in the C library.

        Args:
            dest: A pointer (as a ctypes.c_uint64) to the destination string.
            src: A pointer (as a ctypes.c_uint64) to the source string.

        Returns:
            A pointer (as a ctypes.c_uint64) to the destination string.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.strcpy(self.vm, dest, src)

    @exception_handler
    def strcat(self, dest: ctypes.c_uint64, src: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Concatenates two strings in the C library.

        Args:
            dest: A pointer (as a ctypes.c_uint64) to the destination string.
            src: A pointer (as a ctypes.c_uint64) to the source string.

        Returns:
            A pointer (as a ctypes.c_uint64) to the concatenated string.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.strcat(self.vm, dest, src)

    @exception_handler
    def strrchr(self, src_ptr: ctypes.c_uint64, match_ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Finds the last occurrence of a character in a string in the C library.

        Args:
            src_ptr: A pointer (as a ctypes.c_uint64) to the string to search.
            match_ptr: A pointer (as a ctypes.c_uint64) to the character to match.

        Returns:
            A pointer (as a ctypes.c_uint64) to the last occurrence of the character, or 0 if not found.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.strrchr(self.vm, src_ptr, match_ptr)

    @exception_handler
    def strchr(self, src_ptr: ctypes.c_uint64, match_ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Finds the first occurrence of a character in a string in the C library.

        Args:
            src_ptr: A pointer (as a ctypes.c_uint64) to the string to search.
            match_ptr: A pointer (as a ctypes.c_uint64) to the character to match.

        Returns:
            A pointer (as a ctypes.c_uint64) to the first occurrence of the character, or 0 if not found.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.strchr(self.vm, src_ptr, match_ptr)

    @exception_handler
    def match(self, src_ptr: ctypes.c_uint64, match_ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Checks if a string matches another string in the C library.

        Args:
            src_ptr: A pointer (as a ctypes.c_uint64) to the string to check.
            match_ptr: A pointer (as a ctypes.c_uint64) to the string to match against.

        Returns:
            A status code (typically 0 for no match, non-zero for a match).

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.match(self.vm, src_ptr, match_ptr)

    @exception_handler
    def offset(self, src_ptr: ctypes.c_uint64, match_ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Gets the offset of a substring in a string in the C library.

        Args:
            src_ptr: A pointer (as a ctypes.c_uint64) to the string to search.
            match_ptr: A pointer (as a ctypes.c_uint64) to the substring to find.

        Returns:
            A pointer (as a ctypes.c_uint64) to the offset of the substring, or 0 if not found.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.offset(self.vm, src_ptr, match_ptr)

    @exception_handler
    def load(self, data: ctypes.c_char_p) -> ctypes.c_uint64:
        """
        Loads a string from a C-style string in the C library.

        Args:
            data: A ctypes.c_char_p representing the C-style string to load.

        Returns:
            A pointer (as a ctypes.c_uint64) to the newly loaded string.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.load(self.vm, data)

    @exception_handler
    def put_char(self, string_ptr: ctypes.c_uint64, char: ctypes.c_char) -> None:
        """
        Puts a character into a string in the C library.

        Args:
            string_ptr: A pointer (as a ctypes.c_uint64) to the string.
            char: A ctypes.c_char representing the character to put.

        Returns:
            None

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.put_char(self.vm, string_ptr, char)

    @exception_handler
    def unsafe(self, string_ptr: ctypes.c_uint64) -> ctypes.c_char_p:
        """
        Gets an unsafe pointer to the string data in the C library.

        Args:
            string_ptr: A pointer (as a ctypes.c_uint64) to the string.

        Returns:
            A ctypes.c_char_p representing an unsafe pointer to the string data.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.unsafe(self.vm, string_ptr)

    @exception_handler
    def size(self, ptr: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Gets the size of a string in the C library.

        Args:
            ptr: A pointer (as a ctypes.c_uint64) to the string.

        Returns:
            The size of the string.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.size(self.vm, ptr)

    @exception_handler
    def lessthan(self, src: ctypes.c_uint64, dest: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Checks if one string is less than another string in the C library.

        Args:
            src: A pointer (as a ctypes.c_uint64) to the first string.
            dest: A pointer (as a ctypes.c_uint64) to the second string.

        Returns:
            A status code (typically 0 if not less than, non-zero if less than).

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.lessthan(self.vm, src, dest)

    @exception_handler
    def greaterthan(self, src: ctypes.c_uint64, dest: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Checks if one string is greater than another string in the C library.

        Args:
            src: A pointer (as a ctypes.c_uint64) to the first string.
            dest: A pointer (as a ctypes.c_uint64) to the second string.

        Returns:
            A status code (typically 0 if not greater than, non-zero if greater than).

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.greaterthan(self.vm, src, dest)

    @exception_handler
    def equals(self, src: ctypes.c_uint64, dest: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Checks if two strings are equal in the C library.

        Args:
            src: A pointer (as a ctypes.c_uint64) to the first string.
            dest: A pointer (as a ctypes.c_uint64) to the second string.

        Returns:
            A status code (typically 0 if not equal, non-zero if equal).

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.equals(self.vm, src, dest)

    @exception_handler
    def compare(self, src: ctypes.c_uint64, dest: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Compares two strings in the C library.

        Args:
            src: A pointer (as a ctypes.c_uint64) to the first string.
            dest: A pointer (as a ctypes.c_uint64) to the second string.

        Returns:
            A status code (typically 0 if equal, negative if src < dest, positive if src > dest).

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.compare(self.vm, src, dest)

    @exception_handler
    def left(self, src: ctypes.c_uint64, offset: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Gets the left part of a string in the C library.

        Args:
            src: A pointer (as a ctypes.c_uint64) to the string.
            offset: A ctypes.c_uint64 representing the offset from the left.

        Returns:
            A pointer (as a ctypes.c_uint64) to the left part of the string.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.left(self.vm, src, offset)

    @exception_handler
    def strncpy(self, src: ctypes.c_uint64, nbytes: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Copies a part of a string in the C library.

        Args:
            src: A pointer (as a ctypes.c_uint64) to the string.
            nbytes: A ctypes.c_uint64 representing the number of bytes to copy.

        Returns:
            A pointer (as a ctypes.c_uint64) to the copied part of the string.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.strncpy(self.vm, src, nbytes)

    @exception_handler
    def left_strncpy(self, src: ctypes.c_uint64, offset: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Copies a part of a string from the left in the C library.

        Args:
            src: A pointer (as a ctypes.c_uint64) to the string.
            offset: A ctypes.c_uint64 representing the offset from the left.

        Returns:
            A pointer (as a ctypes.c_uint64) to the copied part of the string.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.left_strncpy(self.vm, src, offset)

    @exception_handler
    def right(self, src: ctypes.c_uint64, offset: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Gets the right part of a string in the C library.

        Args:
            src: A pointer (as a ctypes.c_uint64) to the string.
            offset: A ctypes.c_uint64 representing the offset from the right.

        Returns:
            A pointer (as a ctypes.c_uint64) to the right part of the string.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.right(self.vm, src, offset)

    @exception_handler
    def move_left(self, src: ctypes.c_uint64, nbytes: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Moves a part of a string to the left in the C library.

        Args:
            src: A pointer (as a ctypes.c_uint64) to the string.
            nbytes: A ctypes.c_uint64 representing the number of bytes to move.

        Returns:
            A pointer (as a ctypes.c_uint64) to the modified string.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.move_left(self.vm, src, nbytes)

    @exception_handler
    def move_right(self, src: ctypes.c_uint64, nbytes: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Moves a part of a string to the right in the C library.

        Args:
            src: A pointer (as a ctypes.c_uint64) to the string.
            nbytes: A ctypes.c_uint64 representing the number of bytes to move.

        Returns:
            A pointer (as a ctypes.c_uint64) to the modified string.

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.move_right(self.vm, src, nbytes)

    @exception_handler
    def strcmp(self, src: ctypes.c_uint64, dest: ctypes.c_uint64) -> ctypes.c_uint64:
        """
        Compares two strings in the C library.

        Args:
            src: A pointer (as a ctypes.c_uint64) to the first string.
            dest: A pointer (as a ctypes.c_uint64) to the second string.

        Returns:
            A status code (typically 0 if equal, negative if src < dest, positive if src > dest).

        Raises:
            Exception: If an error occurs during the C library call.
        """
        return self.string_methods.strcmp(self.vm, src, dest)
