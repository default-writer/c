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
    def calloc(self, nmemb, size):
        return self.os_methods.calloc(nmemb, size)


    @classmethod
    def realloc(self, ptr, size):
        return self.os_methods.realloc(ptr, size)


    @classmethod
    def free(self, ptr):
        return self.os_methods.free(ptr)


    @classmethod
    def fclose(self, stream):
        return self.os_methods.fclose(stream)


    @classmethod
    def fopen(self, filename, modes):
        return self.os_methods.fopen(filename, modes)


    @classmethod
    def fread(self, ptr, size, n, stream):
        return self.os_methods.fread(ptr, size, n, stream)


    @classmethod
    def fseek(self, stream, off, whence):
        return self.os_methods.fseek(stream, off, whence)

 
    @classmethod
    def ftell(self, stream):
        return self.os_methods.ftell(stream)

 
    @classmethod
    def getcwd(self, buf, size):
        return self.os_methods.getcwd(buf, size)

 
    @classmethod
    def getenv(self, name):
        return self.os_methods.getenv(name)

 
    @classmethod
    def memcpy(self, dest, src, n):
        return self.os_methods.memcpy(dest, src, n)

 
    @classmethod
    def memset(self, s, c, n):
        return self.os_methods.memset(s, c, n)

 
    @classmethod
    def puts(self, s):
        return self.os_methods.puts(s)

 
    @classmethod
    def strcmp(self, s1, s2):
        return self.os_methods.strcmp(s1, s2)

 
    @classmethod
    def strncat(self, dest, src, n):
        return self.os_methods.strncat(dest, src, n)

 
    @classmethod
    def strncpy(self, dest, src, n):
        return self.os_methods.strncpy(dest, src, n)

 
    @classmethod
    def strlen(self, s):
        return self.os_methods.strlen(s)
