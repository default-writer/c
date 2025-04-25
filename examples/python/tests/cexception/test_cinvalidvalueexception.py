import pytest
from c import CInvalidValueException, CException


def test_invalid_value_exception():
    with pytest.raises(CInvalidValueException) as excinfo:
        raise CInvalidValueException("Invalid value")
    assert "Invalid value" in str(excinfo.value)


def test_invalid_value_exception_catch_cexception():
    try:
        raise CInvalidValueException("Invalid value")
    except CException as e:
        assert "Invalid value" in str(e)
