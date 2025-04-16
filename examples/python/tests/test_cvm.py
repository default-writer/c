# filepath: examples/python/tests/test_cvm.py
import pytest
from c import CVirtualMachine

def test_vm_initialization():
    with CVirtualMachine(1024) as cvm_ptr:
        assert cvm_ptr is not None
