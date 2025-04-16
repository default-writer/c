import pytest
from c import CInvalidTypeIdException

def test_invalid_type_id_exception():
    with pytest.raises(CInvalidTypeIdException) as excinfo:
        raise CInvalidTypeIdException("Invalid type ID")
    assert "Invalid type ID" in str(excinfo.value)
