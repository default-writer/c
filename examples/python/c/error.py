# c/error.py
import ctypes


class CErrorType:
    Default = 0
    VirtualMachineNotInitializedError = 1
    InvalidPointerError = 2
    InvalidArgumentError = 3
    InvalidTypeIdError = 4
    InvalidValueError = 5


class CError:
    class error_methods(ctypes.Structure):
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
        cls.error_methods_ptr = lib.error
        cls.error_methods_ptr.restype = ctypes.POINTER(cls.error_methods)
        cls.error_methods = cls.error_methods_ptr().contents


    @classmethod    
    def stdout(cls):
        return cls.error_methods.stdout()


    @classmethod
    def stderr(cls):
        return cls.error_methods.stderr()


    @classmethod
    def output(cls, output_file, code, message, size):
        cls.error_methods.output(output_file, code, message, size)


    @classmethod
    def throw(cls, code, message, size):
        cls.error_methods.throw(code, message, size)


    @classmethod
    def clear(cls):
        cls.error_methods.clear()


    @classmethod
    def type(cls):
        return cls.error_methods.type()


    @classmethod
    def get(cls):
        return cls.error_methods.get()


class CVirtualMachineNotInitializedException(Exception):
    def __init__(self, message):
        super().__init__(message)


class CInvalidPointerException(Exception):
    def __init__(self, message):
        super().__init__(message)


class CInvalidArgumentException(Exception):
    def __init__(self, message):
        super().__init__(message)


class CInvalidTypeIdException(Exception):
    def __init__(self, message):
        super().__init__(message)


class CInvalidValueException(Exception):
    def __init__(self, message):
        super().__init__(message)


class CException:
    last_error_code: CErrorType = CErrorType.Default
    last_error_message: str = b''

    @classmethod
    def check(self):
        self.last_error_code = CError.type()
        if self.last_error_code:
            self.last_error_message = CError.get().decode()
            self.raise_exception()

    @classmethod
    def raise_exception(self):
        exception = self._get_exception()
        self.last_error_code = CErrorType.Default
        self.last_error_message = b''
        CError.clear()
        raise exception

    @classmethod
    def _get_exception(self):
        if self.last_error_code == CErrorType.VirtualMachineNotInitializedError:
            return CVirtualMachineNotInitializedException(self.last_error_message)
        elif self.last_error_code == CErrorType.InvalidPointer:
            return CInvalidPointerException(self.last_error_message)
        elif self.last_error_code == CErrorType.InvalidArgumentError:
            return CInvalidArgumentException(self.last_error_message)
        elif self.last_error_code == CErrorType.InvalidTypeIdError:
            return CInvalidTypeIdException(self.last_error_message)
        elif self.last_error_code == CErrorType.InvalidValueError:
            return CInvalidValueException(self.last_error_message)
        else:
            return Exception(self.last_error_message)
