# c/os.py
import ctypes

class COperatingSystem:
    class os_methods(ctypes.Structure):
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
            ("memset", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.c_int, ctypes.c_size_t)),
            ("puts", ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_char_p)),
            ("strcmp", ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p)),
            ("strncat", ctypes.CFUNCTYPE(ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_size_t)),
            ("strncpy", ctypes.CFUNCTYPE(ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_size_t)),
            ("strlen", ctypes.CFUNCTYPE(ctypes.c_size_t, ctypes.c_char_p)),
        ]

    @classmethod
    def setup(cls, lib):
        cls.os_methods_ptr = lib.os
        cls.os_methods_ptr.restype = ctypes.POINTER(cls.os_methods)
        cls.os_methods = cls.os_methods_ptr().contents


    @classmethod
    def calloc(self, nmemb: ctypes.c_size_t, size: ctypes.c_size_t) -> ctypes.c_void_p:
        return self.os_methods.calloc(nmemb, size)


    @classmethod
    def realloc(self, ptr: ctypes.c_void_p, size: ctypes.c_size_t) -> ctypes.c_void_p:
        return self.os_methods.realloc(ptr, size)


    @classmethod
    def free(self, ptr: ctypes.c_void_p):
        return self.os_methods.free(ptr)


    @classmethod
    def fclose(self, stream: ctypes.c_void_p) -> ctypes.c_int:
        return self.os_methods.fclose(stream)


    @classmethod
    def fopen(self, filename: ctypes.c_char_p, modes: ctypes.c_char_p) -> ctypes.c_void_p:
        return self.os_methods.fopen(filename, modes)


    @classmethod
    def fread(self, ptr: ctypes.c_void_p, size: ctypes.c_size_t, n: ctypes.c_size_t, stream: ctypes.c_void_p) -> ctypes.c_size_t:
        return self.os_methods.fread(ptr, size, n, stream)


    @classmethod
    def fseek(self, stream: ctypes.c_void_p, off: ctypes.c_long, whence: ctypes.c_int) -> ctypes.c_int:
        return self.os_methods.fseek(stream, off, whence)

 
    @classmethod
    def ftell(self, stream: ctypes.c_void_p) -> ctypes.c_long:
        return self.os_methods.ftell(stream)

 
    @classmethod
    def getcwd(self, buf: ctypes.c_char_p, size: ctypes.c_size_t) -> ctypes.c_char_p:
        return self.os_methods.getcwd(buf, size)

 
    @classmethod
    def getenv(self, name: ctypes.c_char_p) -> ctypes.c_char_p:
        return self.os_methods.getenv(name)

 
    @classmethod
    def memcpy(self, dest: ctypes.c_void_p, src: ctypes.c_void_p, n: ctypes.c_size_t) -> ctypes.c_void_p:
        return self.os_methods.memcpy(dest, src, n)

 
    @classmethod
    def memset(self, s: ctypes.c_void_p, c: ctypes.c_int, n: ctypes.c_size_t) -> ctypes.c_void_p:
        return self.os_methods.memset(s, c, n)

 
    @classmethod
    def puts(self, s: ctypes.c_char_p) -> ctypes.c_int:
        return self.os_methods.puts(s)

 
    @classmethod
    def strcmp(self, s1: ctypes.c_char_p, s2: ctypes.c_char_p) -> ctypes.c_int:
        return self.os_methods.strcmp(s1, s2)

 
    @classmethod
    def strncat(self, dest: ctypes.c_char_p, src: ctypes.c_char_p, n: ctypes.c_size_t) -> ctypes.c_char_p:
        return self.os_methods.strncat(dest, src, n)

 
    @classmethod
    def strncpy(self, dest: ctypes.c_char_p, src: ctypes.c_char_p, n: ctypes.c_size_t) -> ctypes.c_char_p:
        return self.os_methods.strncpy(dest, src, n)

 
    @classmethod
    def strlen(self, s: ctypes.c_char_p) -> ctypes.c_size_t:
        return self.os_methods.strlen(s)
