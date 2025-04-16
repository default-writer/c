import pytest
from c import CInvalidValueException

def test_invalid_value_exception():
    with pytest.raises(CInvalidValueException) as excinfo:
        raise CInvalidValueException("Invalid value")
    assert "Invalid value" in str(excinfo.value)
