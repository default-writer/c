import pytest
from c import CInvalidPointerException, CException


def test_invalid_pointer_exception():
    with pytest.raises(CInvalidPointerException) as excinfo:
        raise CInvalidPointerException("Invalid pointer")
    assert "Invalid pointer" in str(excinfo.value)


def test_invalid_pointer_exception_catch_cexception():
    try:
        raise CInvalidPointerException("Invalid pointer")
    except CException as e:
        assert "Invalid pointer" in str(e)
