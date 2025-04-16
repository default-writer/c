import pytest
from c import CInvalidPointerException

def test_invalid_pointer_exception():
    with pytest.raises(CInvalidPointerException) as excinfo:
        raise CInvalidPointerException("Invalid pointer")
    assert "Invalid pointer" in str(excinfo.value)
