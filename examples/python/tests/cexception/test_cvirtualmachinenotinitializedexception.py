import pytest
from c import CVirtualMachineNotInitializedException, CException


def test_virtual_machine_not_initialized_exception():
    with pytest.raises(CVirtualMachineNotInitializedException) as excinfo:
        raise CVirtualMachineNotInitializedException("VM not initialized")
    assert "VM not initialized" in str(excinfo.value)


def test_virtual_machine_not_initialized_exception_catch_cexception():
    try:
        raise CVirtualMachineNotInitializedException("VM not initialized")
    except CException as e:
        assert "VM not initialized" in str(e)
