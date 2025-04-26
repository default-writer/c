# examples/python/memory/main.py
from c import CVirtualMachine, CString


def test_cstring_load():
    with CVirtualMachine(8) as vm:
        cstring: CString = CString(vm, b"a".decode())
        assert cstring.ptr() == 1


def test_cstring_load_empty():
    with CVirtualMachine(8) as vm:
        cstring: CString = CString(vm, b"".decode())
        assert cstring.ptr() == 1


def test_cstring_size():
    with CVirtualMachine(8) as vm:
        cstring: CString = CString(vm, b"a".decode())
        text_size = CString.size(vm, cstring.ptr())
        assert text_size == 1
