import pytest
from c import CException

def test_exception():
    with pytest.raises(CException) as excinfo:
        raise CException("Unknown exception")
    assert "Unknown exception" in str(excinfo.value)
