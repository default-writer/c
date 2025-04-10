# examples/python/memory/main.py
import ctypes
from c import CVirtualMachine, CPointer, CList, CString, CException, CStack, CEnvironment


def main():
    """
    Demonstrates memory leak handling using CVirtualMachine and CString.

    This example is inspired by the test_vm_dump_memory_leak_2 test case in test_vm.c.
    It shows how to load strings, manipulate them, and potentially encounter memory leaks
    if not handled carefully.
    """
    try:
        try:
            # Create a Virtual Machine instance with a size of 8.
            with CVirtualMachine(8) as cvm_ptr:

                cstring: CString = CString(cvm_ptr)
                cenv: CEnvironment = CEnvironment(cvm_ptr)
                cstack: CStack = CStack(cvm_ptr)
                cpointer: CPointer = CPointer(cvm_ptr)
                clist: CList = CList(cvm_ptr)

                test_data = [
                    b"a",
                    b"a\nb",
                    b"ab\nabc\n",
                    b"adadadsadsadad\ndad\nadsaddasaddad\nsad\n",
                    b"ab\ndad\nabcd\nbcd\n",
                    b"ab\nc\nabc\nbcd\n",
                    b"abc\nabcd\nbcde\nabc\n",
                    b"abc\n\n"
                ]

                for text in test_data:
                    text_ptr = cstring.load(text)
                    parse_text_memory_leak2(text_ptr, cstring, cenv, cstack)
                    cstring.free(text_ptr)

                    list_ptr = clist.init()

                    cvm_ptr.dump_ref_stack(list_ptr.ptr)

                    while (memory_ptr := clist.pop(list_ptr.ptr)):
                        memory_ref_ptr = cpointer.ref(memory_ptr)
                        print(f"[  v< ]: {memory_ptr:016x}")
                        print(f"[  v& ]: {memory_ref_ptr:016x}")
                        cpointer.free(memory_ref_ptr)

                    clist.destroy(list_ptr)

        except CException as e:
            # Catch any exceptions that occur during the process and print an error message.
            print(f"error: {e}")

    except Exception as e:
        # Catch any exceptions that occur during the process and print an error message.
        print(f"error: {e}")


def parse_text_memory_leak2(text_ptr, cstring: CString, cenv: CEnvironment, cstack: CStack):
    text_size = cstring.size(text_ptr)
    if text_size == 0:
        return

    text = cstring.unsafe(text_ptr)
    tmp = text.decode()
    lines = tmp.split('\n')
    
    stack_ptr1 = cstack.alloc()

    for line in lines:
        if line:
            string_ptr = cstring.load(line.encode())
            cstack.push(stack_ptr1, string_ptr)

    stack_ptr2 = cstack.alloc()
    while cstack.size(stack_ptr1) > 0:
        string_ptr = cstack.pop(stack_ptr1)
        cstack.push(stack_ptr2, string_ptr)
    cstack.free(stack_ptr1)
   
    quit = 0
    while quit == 0 and cstack.size(stack_ptr2) > 0:
        string_ptr = cstack.pop(stack_ptr2)
        if string_ptr == 0:
            quit = 1
            continue
        cenv.puts(string_ptr)
        pattern_ptr = cstack.pop(stack_ptr2)
        if pattern_ptr == 0:
            quit = 1
            continue
        
        cenv.puts(pattern_ptr)
        size = cstring.size(pattern_ptr)
        string_pointer_ptr = 0
        current_ptr = string_ptr
        while (string_pointer_ptr := cstring.strchr(current_ptr, pattern_ptr, nothrow=True)) != 0:
            match_ptr = cstring.match(string_pointer_ptr, pattern_ptr, nothrow=True)
            if match_ptr == 0:
                break
            if cstring.lessthan(string_pointer_ptr, match_ptr, nothrow=True):
                match_start_ptr = cstring.left(match_ptr, size, nothrow=True)
                str_ncpy = cstring.strncpy(match_start_ptr, size, nothrow=True)
                distance = cstring.lessthan(string_ptr, match_start_ptr, nothrow=True)
                if distance > 0:
                    substring_text = cstring.unsafe(str_ncpy).decode()
                    print(" " * (distance - 1), f"{substring_text}[{distance}]")
            current_ptr = match_ptr

    cstack.free(stack_ptr2)

if __name__ == "__main__":
    main()
