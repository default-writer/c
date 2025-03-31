import ctypes
import os

def load_libc_sys():
    lib_path = os.path.join("lib", "libc-sys.so")
    try:
        c = ctypes.CDLL(lib_path)
        return c
    except OSError as e:
        print(f"Error loading library: {e}")
        return None

def load_libc_vm():
    lib_path = os.path.join("lib", "libc-vm.so")
    try:
        c = ctypes.CDLL(lib_path)
        return c
    except OSError as e:
        print(f"Error loading library: {e}")
        return None


c_sys = load_libc_sys()
c_vm = load_libc_vm()

class VM:
    class vm_methods(ctypes.Structure):
        _fields_ = [
            ("init", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("gc", ctypes.CFUNCTYPE(None)),
            ("release", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.c_uint64)),
            ("destroy", ctypes.CFUNCTYPE(None)),
        ]

    def __init__(self):
        vm_methods_ptr = c_vm.vm
        vm_methods_ptr.restype = ctypes.POINTER(VM.vm_methods)
        self.vm_methods = vm_methods_ptr().contents

    def init(self, size):
        return self.vm_methods.init(size)

    def gc(self):
        self.vm_methods.gc()

    def release(self, ptr):
        self.vm_methods.release(ptr)

    def destroy(self):
        self.vm_methods.destroy()

class OS:
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

    def __init__(self):
        os_methods_ptr = c_sys.os
        os_methods_ptr.restype = ctypes.POINTER(OS.os_methods)
        self.os_methods = os_methods_ptr().contents

    def calloc(self, nmemb, size):
        return self.os_methods.calloc(nmemb, size)

    def realloc(self, ptr, size):
        return self.os_methods.realloc(ptr, size)

    def free(self, ptr):
        return self.os_methods.free(ptr)

    def fclose(self, stream):
        return self.os_methods.fclose(stream)

    def fopen(self, filename, modes):
        return self.os_methods.fopen(filename, modes)

    def fread(self, ptr, size, n, stream):
        return self.os_methods.fread(ptr, size, n, stream)

    def fseek(self, stream, off, whence):
        return self.os_methods.fseek(stream, off, whence)

    def ftell(self, stream):
        return self.os_methods.ftell(stream)

    def getcwd(self, buf, size):
        return self.os_methods.getcwd(buf, size)

    def getenv(self, name):
        return self.os_methods.getenv(name)

    def memcpy(self, dest, src, n):
        return self.os_methods.memcpy(dest, src, n)

    def memset(self, s, c, n):
        return self.os_methods.memset(s, c, n)

    def puts(self, s):
        return self.os_methods.puts(s)

    def strcmp(self, s1, s2):
        return self.os_methods.strcmp(s1, s2)

    def strncat(self, dest, src, n):
        return self.os_methods.strncat(dest, src, n)

    def strncpy(self, dest, src, n):
        return self.os_methods.strncpy(dest, src, n)

    def strlen(self, s):
        return self.os_methods.strlen(s)

class MEMORY:
    class memory_methods(ctypes.Structure):
        _fields_ = [
            ("alloc", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
            ("realloc", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("free", ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64)),
        ]

    def __init__(self):
        memory_methods_ptr = c_sys.memory
        memory_methods_ptr.restype = ctypes.POINTER(MEMORY.memory_methods)
        self.memory_methods = memory_methods_ptr().contents

    def alloc(self, size):
        return self.memory_methods.alloc(size)

    def realloc(self, ptr, size, new_size):
        return self.memory_methods.copy(ptr, size, new_size)

    def free(self, ptr, size):
        return self.memory_methods.free(ptr, size)

class POINTER:
    class pointer_methods(ctypes.Structure):
        _fields_ = [
            ("alloc", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("copy", ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64, ctypes.c_uint64)),
            ("read", ctypes.CFUNCTYPE(ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_uint64)),
            ("free", ctypes.CFUNCTYPE(None, ctypes.c_uint64, ctypes.c_uint64)),
        ]

    def __init__(self):
        pointer_methods_ptr = c_vm.pointer
        pointer_methods_ptr.restype = ctypes.POINTER(POINTER.pointer_methods)
        self.pointer_methods = pointer_methods_ptr().contents

    def alloc(self, data, size, type_id):
        return self.pointer_methods.alloc(data, size, type_id)

    def copy(self, src, size, offset, type_id):
        return self.pointer_methods.copy(src, size, offset, type_id)

    def read(self, address, type_id):
        return self.pointer_methods.read(address, type_id)

    def free(self, address, type_id):
        return self.pointer_methods.free(address, type_id)

class STRING:
    class string_methods(ctypes.Structure):
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

    def __init__(self):
        string_methods_ptr = c_vm.string
        string_methods_ptr.restype = ctypes.POINTER(STRING.string_methods)
        self.string_methods = string_methods_ptr().contents

    def free(self, vm, ptr):
        return self.string_methods.free(vm, ptr)

    def copy(self, vm, ptr):
        return self.string_methods.copy(vm, ptr)

    def strcpy(self, vm, dest, src):
        return self.string_methods.strcpy(vm, dest, src)

    def strcat(self, vm, dest, src):
        return self.string_methods.strcat(vm, dest, src)

    def strrchr(self, vm, src_ptr, match_ptr):
        return self.string_methods.strrchr(vm, src_ptr, match_ptr)

    def strchr(self, vm, src_ptr, match_ptr):
        return self.string_methods.strchr(vm, src_ptr, match_ptr)

    def match(self, vm, src_ptr, match_ptr):
        return self.string_methods.match(vm, src_ptr, match_ptr)

    def offset(self, vm, src_ptr, match_ptr):
        return self.string_methods.offset(vm, src_ptr, match_ptr)

    def load(self, vm, data):
        return self.string_methods.load(vm, data)

    def put_char(self, vm, string_ptr, char):
        self.string_methods.put_char(vm, string_ptr, char)

    def unsafe(self, vm, string_ptr):
        return self.string_methods.unsafe(vm, string_ptr)

    def size(self, vm, ptr):
        return self.string_methods.size(vm, ptr)

    def lessthan(self, vm, src, dest):
        return self.string_methods.lessthan(vm, src, dest)

    def greaterthan(self, vm, src, dest):
        return self.string_methods.greaterthan(vm, src, dest)

    def equals(self, vm, src, dest):
        return self.string_methods.equals(vm, src, dest)

    def compare(self, vm, src, dest):
        return self.string_methods.compare(vm, src, dest)

    def left(self, vm, src, offset):
        return self.string_methods.left(vm, src, offset)

    def strncpy(self, vm, src, nbytes):
        return self.string_methods.strncpy(vm, src, nbytes)

    def left_strncpy(self, vm, src, offset):
        return self.string_methods.left_strncpy(vm, src, offset)

    def right(self, vm, src, offset):
        return self.string_methods.right(vm, src, offset)

    def move_left(self, vm, src, nbytes):
        return self.string_methods.move_left(vm, src, nbytes)

    def move_right(self, vm, src, nbytes):
        return self.string_methods.move_right(vm, src, nbytes)

    def strcmp(self, vm, src, dest):
        return self.string_methods.strcmp(vm, src, dest)

class TYPE:
    _type_map = {}

    @classmethod
    def init(cls):
        # TYPE._add_type("NULL", 0)
        # TYPE._add_type("DATA", 1)
        TYPE._add_type("STRING", 2)
        # TYPE._add_type("STRING_POINTER", 3)
        # TYPE._add_type("FILE", 4)
        # TYPE._add_type("STACK", 5)
        # TYPE._add_type("OBJECT", 6)
        # TYPE._add_type("USER", 7)

    @classmethod
    def _add_type(cls, type_name, type_id):
        """Adds a new type to the enumeration."""
        if type_name in cls._type_map:
            raise ValueError(f"Type '{type_name}' already exists.")
        if type_id in cls._type_map.values():
            raise ValueError(f"Type ID '{type_id}' already in use.")
        setattr(cls, type_name, type_id)
        cls._type_map[type_name] = type_id

    @classmethod
    def get_type_id(cls, type_name):
        """Gets the type ID for a given type name."""
        return cls._type_map.get(type_name, None)

    @classmethod
    def is_known_type(cls, type_id):
        """Checks if a type ID is a known type."""
        return type_id in cls._type_map.values()

    @classmethod
    def is_known_type_name(cls, type_name):
        """Checks if a type name is a known type."""
        return type_name in cls._type_map

    @classmethod
    def get_type_name(cls, type_id):
        """Gets the type name for a given type ID."""
        for type_name, tid in cls._type_map.items():
            if tid == type_id:
                return type_name
        return None

TYPE.init()

class ERROR:
    class error_methods(ctypes.Structure):
        _fields_ = [
            ("output", ctypes.CFUNCTYPE(None, ctypes.POINTER(ctypes.c_void_p), ctypes.c_uint64, ctypes.c_char_p, ctypes.c_uint64)),
            ("throw", ctypes.CFUNCTYPE(None, ctypes.c_uint64, ctypes.c_char_p, ctypes.c_uint64)),
            ("clear", ctypes.CFUNCTYPE(None)),
            ("has", ctypes.CFUNCTYPE(ctypes.c_uint64)),
        ]

    def __init__(self):
        error_methods_ptr = c_sys.error
        error_methods_ptr.restype = ctypes.POINTER(ERROR.error_methods)
        self.error_methods = error_methods_ptr().contents

    def output(self, output_file, code, message, size):
        self.error_methods.output(output_file, code, message, size)

    def throw(self, code, message, size):
        self.error_methods.throw(code, message, size)

    def clear(self):
        self.error_methods.clear()

    def has(self):
        return self.error_methods.has()

def main():
    try:
        vm = VM()
        vm_ptr = vm.init(8)

        if vm_ptr:
            try:
                os = OS()
                memory = MEMORY()
                pointer = POINTER()
                string = STRING()
                error = ERROR()

                dot = memory.alloc(1)
                str_ptr = pointer.alloc(dot, 1, TYPE.STRING)

                dot_bytes = b"."
                string.put_char(vm_ptr, str_ptr, dot_bytes)

                string = STRING()
                string.load(vm_ptr, b"hello, world!")
                os.puts(string.unsafe(vm_ptr, str_ptr))
                os.puts(b"hello, world!")
                
                # Example usage of ERROR class
                error.throw(1, b"VM not initialized", len(b"VM not initialized"))
                if error.has():
                    error.output(ctypes.c_void_p(1), 1, b"VM not initialized", len(b"VM not initialized"))
                    error.clear()

                # os.free(dot) -- double free or corruption, do not use directly, you should understand what are you doing
            except Exception as e:
                print(f"error: {e}")
        else:
            print("error: init")

        vm.gc()
        vm.destroy()

    except Exception as e:
        print(f"error: {e}")


if __name__ == "__main__":
    main()
