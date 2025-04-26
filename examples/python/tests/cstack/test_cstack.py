from c import CVirtualMachine, CStack, CString, CException


def test_stack_init():
    with CVirtualMachine(1024) as vm:
        stack = CStack(vm)
        assert stack is not None


def test_stack_push():
    with CVirtualMachine(1024) as vm:
        stack = CStack(vm)
        stack.push(42)  # Push an element onto the stack


def test_stack_pop():
    with CVirtualMachine(1024) as vm:
        stack = CStack(vm)
        stack.push(42)
        value = stack.pop()
        assert value == 42


def test_stack_peek():
    with CVirtualMachine(1024) as vm:
        stack = CStack(vm)
        stack.push(42)
        value = stack.peek()
        assert value == 42


def test_stack_peek_0():
    try:
        with CVirtualMachine(1024) as vm:
            stack = CStack(vm)
            value = stack.peek()
    except CException as e:
        assert "no elements to peek in the stack" in str(e)


def test_stack_peekn():
    with CVirtualMachine(1024) as vm:
        stack = CStack(vm)
        stack.push(42)
        stack.push(43)
        new_stack_ptr = stack.peekn(2)
        assert new_stack_ptr != 0
        stack2 = CStack(vm, new_stack_ptr)
        assert stack.size() == 2
        assert stack2.size() == 2


def test_stack_popn():
    with CVirtualMachine(1024) as vm:
        stack = CStack(vm)
        stack.push(42)
        stack.push(43)
        new_stack_ptr = stack.popn(2)
        assert new_stack_ptr != 0
        stack2 = CStack(vm, new_stack_ptr)
        assert stack.size() == 0
        assert stack2.size() == 2


def test_stack_size():
    with CVirtualMachine(1024) as vm:
        stack = CStack(vm)
        assert stack.size() == 0
        stack.push(42)
        assert stack.size() == 1


def test_stack_push_string():
    with CVirtualMachine(1024) as vm:
        cstring: CString = CString(vm, b"a".decode())
        assert cstring.ptr() == 1
        stack = CStack(vm)
        stack.push(cstring.ptr())
        assert stack.size() == 1


def test_stack_release_string():
    with CVirtualMachine(1024) as vm:
        with CString(vm, b"a".decode()) as cstring:
            with CStack(vm) as stack:
                stack.push(cstring.ptr())
            assert cstring.ptr() == 1


def test_stack_context_manager():
    with CVirtualMachine(1024) as vm:
        with CStack(vm) as stack:
            assert stack is not None
