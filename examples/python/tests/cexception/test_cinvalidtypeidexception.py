import pytest
from c import CInvalidTypeIdException, CException


def test_invalid_type_id_exception():
    with pytest.raises(CInvalidTypeIdException) as excinfo:
        raise CInvalidTypeIdException("Invalid type ID")
    assert "Invalid type ID" in str(excinfo.value)


def test_invalid_type_id_exception_catch_cexception():
    try:
        raise CInvalidTypeIdException("Invalid type ID")
    except CException as e:
        assert "Invalid type ID" in str(e)
