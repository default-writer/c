import pytest
from c import CInvalidArgumentException, CException


def test_invalid_type_id_exception():
    with pytest.raises(CInvalidArgumentException) as excinfo:
        raise CInvalidArgumentException("Invalid argument")
    assert "Invalid argument" in str(excinfo.value)


def test_invalid_type_id_exception_catch_cexception():
    try:
        raise CInvalidArgumentException("Invalid argument")
    except CException as e:
        assert "Invalid argument" in str(e)
