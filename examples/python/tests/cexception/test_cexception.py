import pytest
from c import CException


def test_cexception():
    with pytest.raises(CException) as excinfo:
        raise CException("Unknown exception")
    assert "Unknown exception" in str(excinfo.value)


def test_cexception_catch_exception():
    try:
        raise CException("Unknown exception")
    except Exception as e:
        assert "Unknown exception" in str(e)
