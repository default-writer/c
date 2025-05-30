import traceback

from c import (
    CException,
    CVirtualMachine,
    CPointer,
    CList,
    CString,
    CEnvironment,
)


def test_vm_dump_memory_leak_2():
    test_data = [
        b"a",
        b"a\nb",
        b"ab\nabc\n",
        b"adadadsadsadad\ndad\nadsaddasaddad\nsad\n",
        b"ab\ndad\nabcd\nbcd\n",
        b"ab\nc\nabc\nbcd\n",
        b"abc\nabcd\nbcde\nabc\n",
        b"abc\n\n",
    ]
    debug_cvm_stack: CList
    with CVirtualMachine(8) as debug_cvm:
        for text_bytes in test_data:

            debug_text_string = CString(debug_cvm, text_bytes.decode())
            parse_text_memory_leak2(debug_cvm, debug_text_string.ptr())
            CString.free(debug_cvm, debug_text_string.ptr())

            debug_cvm_stack = CList()
            debug_cvm.dump_ref_stack(debug_cvm_stack)

            current = 0
            while debug_cvm_stack.size() > 0 and (current := debug_cvm_stack.pop()) > 0:
                memory_ref_ptr = CPointer.ref(debug_cvm, current, nothrow=True)
                print(f"[  v< ]: {current:016x}")
                print(f"[  v& ]: {memory_ref_ptr:016x}")
                CPointer.free(debug_cvm, memory_ref_ptr)

            del debug_cvm_stack
        debug_cvm.gc()


def parse_text_memory_leak2(vm: CVirtualMachine, text_string_ptr):
    stack_ptr1: CList = CList()
    if not CString.split(vm, text_string_ptr, stack_ptr1):
        while stack_ptr1.size() > 0 and (string_ptr := stack_ptr1.pop()) > 0:
            CString.free(vm, string_ptr)
        del stack_ptr1
        return
    stack_ptr2: CList = CList()
    while stack_ptr2.size() > 0 and (data_ptr := stack_ptr1.pop()) > 0:
        stack_ptr2.push(data_ptr)
    quit = False
    while not quit:
        string_ptr = stack_ptr2.pop() if stack_ptr2.size() > 0 else 0
        if string_ptr == 0 or CString.size(vm, string_ptr) == 0:
            quit = True
            continue
        CEnvironment.puts(vm, string_ptr)
        pattern_ptr = stack_ptr2.pop() if stack_ptr2.size() > 0 else 0
        if pattern_ptr == 0 or CString.size(vm, pattern_ptr) == 0:
            quit = True
            continue
        CEnvironment.puts(vm, pattern_ptr)
        size = CString.size(vm, pattern_ptr)
        string_reference_ptr = 0
        current_ptr = string_ptr
        while (string_reference_ptr := CString.strchr(vm, current_ptr, pattern_ptr, nothrow=True)) > 0:
            match_ptr = CString.match(vm, string_reference_ptr, pattern_ptr, nothrow=True)
            if match_ptr == 0:
                break
            if CString.lessthan(vm, string_reference_ptr, match_ptr, nothrow=True):
                match_start_ptr = CString.left(vm, match_ptr, size, nothrow=True)
                str_ncpy_ptr = CString.right_copy(vm, match_start_ptr, size, nothrow=True)
                if str_ncpy_ptr != 0:
                    distance = CString.lessthan(vm, string_ptr, match_start_ptr, nothrow=True)
                    print(f"{' ' * distance if distance > 0 else ''}{CString.unsafe(vm, str_ncpy_ptr)}[{distance}]")
                    CString.free(vm, str_ncpy_ptr)
            # CString.free(vm, current_ptr)
            current_ptr = match_ptr
        CString.free(vm, current_ptr)
    del stack_ptr2


def main():
    try:
        test_vm_dump_memory_leak_2()
    except CException as e:
        traceback.print_exc()
        print(f"error: {e}")
    except Exception as e:
        traceback.print_exc()
        print(f"error: {e}")


if __name__ == "__main__":
    main()
