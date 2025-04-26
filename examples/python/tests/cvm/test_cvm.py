# filepath: examples/python/tests/test_cvm.py
import pytest
from c import CVirtualMachine


def test_vm_initialization():
    with CVirtualMachine(1024) as vm:
        assert vm is not None
