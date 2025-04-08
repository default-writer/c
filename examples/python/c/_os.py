# c/os.py
import ctypes


class COperatingSystem:
    """
    COperatingSystem class provides an interface to interact with various operating system
    level functions in a C library.

    It allows performing operations such as memory allocation, file operations,
    string manipulation, and environment variable access.
    """

    class os_methods(ctypes.Structure):
        """
        Structure representing the function pointers for operating system operations in the C library.

        _fields_:
            calloc: Function pointer for allocating and zero-initializing memory.
            realloc: Function pointer for reallocating memory.
            free: Function pointer for freeing memory.
            fclose: Function pointer for closing a file stream.
            fopen: Function pointer for opening a file.
            fread: Function pointer for reading data from a file.
            fseek: Function pointer for repositioning the file pointer.
            ftell: Function pointer for getting the current file pointer position.
            getcwd: Function pointer for getting the current working directory.
            getenv: Function pointer for getting an environment variable.
            memcpy: Function pointer for copying memory.
            memmove: Function pointer for moving memory.
            memset: Function pointer for setting memory to a specific value.
            puts: Function pointer for printing a string to standard output.
            strcmp: Function pointer for comparing two strings.
            strncat: Function pointer for concatenating two strings with a size limit.
            strncpy: Function pointer for copying a string with a size limit.
            strlen: Function pointer for getting the length of a string.
        """
        _fields_ = [
            ("calloc", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.c_size_t, ctypes.c_size_t)),
            ("realloc", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.c_size_t)),
            ("free", ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_void_p))),
            ("fclose", ctypes.CFUNCTYPE(ctypes.c_int, ctypes.POINTER(ctypes.c_void_p))),
            ("fopen", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.c_char_p, ctypes.c_char_p)),
            ("fread", ctypes.CFUNCTYPE(ctypes.c_size_t, ctypes.POINTER(ctypes.c_void_p), ctypes.c_size_t, ctypes.c_size_t, ctypes.POINTER(ctypes.c_void_p))),
            ("fseek", ctypes.CFUNCTYPE(ctypes.c_int, ctypes.POINTER(ctypes.c_void_p), ctypes.c_long, ctypes.c_int)),
            ("ftell", ctypes.CFUNCTYPE(ctypes.c_long, ctypes.POINTER(ctypes.c_void_p))),
            ("getcwd", ctypes.CFUNCTYPE(ctypes.c_char_p, ctypes.c_char_p, ctypes.c_size_t)),
            ("getenv", ctypes.CFUNCTYPE(ctypes.c_char_p, ctypes.c_char_p)),
            ("memcpy", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.c_size_t)),
            ("memmove", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.c_size_t)),
            ("memset", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.c_int, ctypes.c_size_t)),
            ("puts", ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_char_p)),
            ("strcmp", ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p)),
            ("strncat", ctypes.CFUNCTYPE(ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_size_t)),
            ("strncpy", ctypes.CFUNCTYPE(ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_size_t)),
            ("strlen", ctypes.CFUNCTYPE(ctypes.c_size_t, ctypes.c_char_p)),
        ]

    @classmethod
    def setup(cls, lib):
        """
        Sets up the COperatingSystem class by linking it to the C library's OS functions.

        Args:
            lib: The loaded C library object (e.g., from ctypes.CDLL).

        Raises:
            AttributeError: If the C library does not have an 'os' attribute.
        """
        cls.os_methods_ptr = lib.os
        cls.os_methods_ptr.restype = ctypes.POINTER(cls.os_methods)
        cls.os_methods = cls.os_methods_ptr().contents

    @classmethod
    def calloc(cls, nmemb: ctypes.c_size_t, size: ctypes.c_size_t) -> ctypes.c_void_p:
        """
        Allocates a memory block for an array of 'nmemb' elements, each 'size' bytes long,
        and initializes all bits to zero.

        Args:
            nmemb: The number of elements to allocate.
            size: The size of each element in bytes.

        Returns:
            A ctypes.c_void_p representing a pointer to the allocated memory block, or None on failure.
        """
        return cls.os_methods.calloc(nmemb, size)

    @classmethod
    def realloc(cls, ptr: ctypes.c_void_p, size: ctypes.c_size_t) -> ctypes.c_void_p:
        """
        Reallocates a memory block to a new size.

        Args:
            ptr: A ctypes.c_void_p representing a pointer to the previously allocated memory block.
            size: The new size of the memory block.

        Returns:
            A ctypes.c_void_p representing a pointer to the reallocated memory block, or None on failure.
        """
        return cls.os_methods.realloc(ptr, size)

    @classmethod
    def free(cls, ptr: ctypes.c_void_p):
        """
        Frees a memory block previously allocated by calloc or realloc.

        Args:
            ptr: A ctypes.c_void_p representing a pointer to the memory block to free.
        """
        return cls.os_methods.free(ptr)

    @classmethod
    def fclose(cls, stream: ctypes.c_void_p) -> ctypes.c_int:
        """
        Closes a file stream.

        Args:
            stream: A ctypes.c_void_p representing a pointer to the file stream.

        Returns:
            0 on success, EOF on error.
        """
        return cls.os_methods.fclose(stream)

    @classmethod
    def fopen(cls, filename: ctypes.c_char_p, modes: ctypes.c_char_p) -> ctypes.c_void_p:
        """
        Opens a file.

        Args:
            filename: A ctypes.c_char_p representing the name of the file to open.
            modes: A ctypes.c_char_p representing the file access mode (e.g., "r", "w", "a").

        Returns:
            A ctypes.c_void_p representing a pointer to the file stream, or None on failure.
        """
        return cls.os_methods.fopen(filename, modes)

    @classmethod
    def fread(cls, ptr: ctypes.c_void_p, size: ctypes.c_size_t, n: ctypes.c_size_t, stream: ctypes.c_void_p) -> ctypes.c_size_t:
        """
        Reads data from a file stream.

        Args:
            ptr: A ctypes.c_void_p representing a pointer to the buffer to store the data.
            size: The size of each element to read.
            n: The number of elements to read.
            stream: A ctypes.c_void_p representing a pointer to the file stream.

        Returns:
            The number of elements successfully read.
        """
        return cls.os_methods.fread(ptr, size, n, stream)

    @classmethod
    def fseek(cls, stream: ctypes.c_void_p, off: ctypes.c_long, whence: ctypes.c_int) -> ctypes.c_int:
        """
        Repositions the file pointer in a file stream.

        Args:
            stream: A ctypes.c_void_p representing a pointer to the file stream.
            off: The offset in bytes.
            whence: The position from which to offset (e.g., SEEK_SET, SEEK_CUR, SEEK_END).

        Returns:
            0 on success, non-zero on error.
        """
        return cls.os_methods.fseek(stream, off, whence)

    @classmethod
    def ftell(cls, stream: ctypes.c_void_p) -> ctypes.c_long:
        """
        Gets the current file pointer position in a file stream.

        Args:
            stream: A ctypes.c_void_p representing a pointer to the file stream.

        Returns:
            The current file pointer position, or -1 on error.
        """
        return cls.os_methods.ftell(stream)

    @classmethod
    def getcwd(cls, buf: ctypes.c_char_p, size: ctypes.c_size_t) -> ctypes.c_char_p:
        """
        Gets the current working directory.

        Args:
            buf: A ctypes.c_char_p representing a pointer to the buffer to store the directory path.
            size: The size of the buffer.

        Returns:
            A ctypes.c_char_p representing a pointer to the directory path, or None on error.
        """
        return cls.os_methods.getcwd(buf, size)

    @classmethod
    def getenv(cls, name: ctypes.c_char_p) -> ctypes.c_char_p:
        """
        Gets the value of an environment variable.

        Args:
            name: A ctypes.c_char_p representing the name of the environment variable.

        Returns:
            A ctypes.c_char_p representing a pointer to the environment variable value, or None if not found.
        """
        return cls.os_methods.getenv(name)

    @classmethod
    def memcpy(cls, dest: ctypes.c_void_p, src: ctypes.c_void_p, n: ctypes.c_size_t) -> ctypes.c_void_p:
        """
        Copies 'n' bytes from memory area 'src' to memory area 'dest'.

        Args:
            dest: A ctypes.c_void_p representing a pointer to the destination memory area.
            src: A ctypes.c_void_p representing a pointer to the source memory area.
            n: The number of bytes to copy.

        Returns:
            A ctypes.c_void_p representing a pointer to the destination memory area.
        """
        return cls.os_methods.memcpy(dest, src, n)

    @classmethod
    def memmove(cls, dest: ctypes.c_void_p, src: ctypes.c_void_p, n: ctypes.c_size_t) -> ctypes.c_void_p:
        """
        Moves 'n' bytes from memory area 'src' to memory area 'dest'.

        Args:
            dest: A ctypes.c_void_p representing a pointer to the destination memory area.
            src: A ctypes.c_void_p representing a pointer to the source memory area.
            n: The number of bytes to move.

        Returns:
            A ctypes.c_void_p representing a pointer to the destination memory area.
        """
        return cls.os_methods.memmove(dest, src, n)

    @classmethod
    def memset(cls, s: ctypes.c_void_p, c: ctypes.c_int, n: ctypes.c_size_t) -> ctypes.c_void_p:
        """
        Sets the first 'n' bytes of the memory area pointed to by 's' to the value 'c'.

        Args:
            s: A ctypes.c_void_p representing a pointer to the memory area.
            c: The value to set.
            n: The number of bytes to set.

        Returns:
            A ctypes.c_void_p representing a pointer to the memory area.
        """
        return cls.os_methods.memset(s, c, n)

    @classmethod
    def puts(cls, s: ctypes.c_char_p) -> ctypes.c_int:
        """
        Writes a string to standard output.

        Args:
            s: A ctypes.c_char_p representing the string to write.

        Returns:
            A non-negative value on success, EOF on error.
        """
        return cls.os_methods.puts(s)

    @classmethod
    def strcmp(cls, s1: ctypes.c_char_p, s2: ctypes.c_char_p) -> ctypes.c_int:
        """
        Compares two strings.

        Args:
            s1: A ctypes.c_char_p representing the first string.
            s2: A ctypes.c_char_p representing the second string.

        Returns:
            An integer less than, equal to, or greater than zero if s1 is found, respectively,
            to be less than, to match, or be greater than s2.
        """
        return cls.os_methods.strcmp(s1, s2)

    @classmethod
    def strncat(cls, dest: ctypes.c_char_p, src: ctypes.c_char_p, n: ctypes.c_size_t) -> ctypes.c_char_p:
        """
        Appends at most 'n' characters from string 'src' to string 'dest',
        null-terminating the result.

        Args:
            dest: A ctypes.c_char_p representing a pointer to the destination string.
            src: A ctypes.c_char_p representing a pointer to the source string.
            n: The maximum number of characters to append.

        Returns:
            A ctypes.c_char_p representing a pointer to the destination string.
        """
        return cls.os_methods.strncat(dest, src, n)

    @classmethod
    def strncpy(cls, dest: ctypes.c_char_p, src: ctypes.c_char_p, n: ctypes.c_size_t) -> ctypes.c_char_p:
        """
        Copies at most 'n' characters from string 'src' to string 'dest'.

        Args:
            dest: A ctypes.c_char_p representing a pointer to the destination string.
            src: A ctypes.c_char_p representing a pointer to the source string.
            n: The maximum number of characters to copy.

        Returns:
            A ctypes.c_char_p representing a pointer to the destination string.
        """
        return cls.os_methods.strncpy(dest, src, n)

    @classmethod
    def strlen(cls, s: ctypes.c_char_p) -> ctypes.c_size_t:
        """
        Calculates the length of a string.

        Args:
            s: A ctypes.c_char_p representing the string.

        Returns:
            The length of the string, excluding the null terminator.
        """
        return cls.os_methods.strlen(s)
