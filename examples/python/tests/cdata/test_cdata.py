from c import CVirtualMachine, CData, CException


def test_data_init():
    with CVirtualMachine(1024) as vm:
        data = CData(vm, 1)
        assert data is not None


def test_data_catch_exception():
    try:
        with CVirtualMachine(1024) as vm:
            data = CData(vm, 0)
    except CException as e:
        assert "failed to allocate data block: invalid size or insufficient memory" in str(e)
