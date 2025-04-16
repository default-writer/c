import pytest
from c import CVirtualMachineNotInitializedException

def test_virtual_machine_not_initialized_exception():
    with pytest.raises(CVirtualMachineNotInitializedException) as excinfo:
        raise CVirtualMachineNotInitializedException("VM not initialized")
    assert "VM not initialized" in str(excinfo.value)
