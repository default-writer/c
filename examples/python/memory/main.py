# examples/python/memory/main.py
import ctypes

from c import CVirtualMachine, CString, CException, CStack, CEnvironment


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
            cvm_ptr: CVirtualMachine = CVirtualMachine(8)

            string: CString = CString(cvm_ptr)
            env: CEnvironment = CEnvironment(cvm_ptr)
            stack: CStack = CStack(cvm_ptr)

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
                text_ptr = string.load(text)
                parse_text_memory_leak2(text_ptr, string, env, stack)
                string.free(text_ptr)

        except CException as e:
            # Catch any exceptions that occur during the process and print an error message.
            print(f"error: {e}")

        finally:
            # Perform garbage collection on the VM.
            cvm_ptr.gc()
            # Delete the VM instance.
            del cvm_ptr

    except Exception as e:
        # Catch any exceptions that occur during the process and print an error message.
        print(f"error: {e}")


def parse_text_memory_leak2(text_ptr, string: CString, env: CEnvironment, stack: CStack):
    text_size = string.size(text_ptr)
    if text_size == 0:
        return

    text = string.unsafe(text_ptr)
    tmp = text.decode()
    lines = tmp.split('\n')
    
    stack_ptr1 = stack.alloc()

    for line in lines:
        if line:
            string_ptr = string.load(line.encode())
            stack.push(stack_ptr1, string_ptr)

    stack_ptr2 = stack.alloc()
    while stack.size(stack_ptr1) > 0:
        string_ptr = stack.pop(stack_ptr1)
        stack.push(stack_ptr2, string_ptr)
    stack.free(stack_ptr1)
   
    quit = 0
    while quit == 0 and stack.size(stack_ptr2) > 0:
        string_ptr = stack.pop(stack_ptr2)
        if string_ptr == 0:
            quit = 1
            continue
        env.puts(string_ptr)
        pattern_ptr = stack.pop(stack_ptr2)
        if pattern_ptr == 0:
            quit = 1
            continue
        
        env.puts(pattern_ptr)
        size = string.size(pattern_ptr)
        string_pointer_ptr = 0
        current_ptr = string_ptr
        while (string_pointer_ptr := string.strchr(current_ptr, pattern_ptr, nothrow=True)) != 0:
            match_ptr = string.match(string_pointer_ptr, pattern_ptr, nothrow=True)
            if match_ptr == 0:
                break
            if string.lessthan(string_pointer_ptr, match_ptr, nothrow=True):
                match_start_ptr = string.left(match_ptr, size, nothrow=True)
                str_ncpy = string.strncpy(match_start_ptr, size, nothrow=True)
                distance = string.lessthan(string_ptr, match_start_ptr, nothrow=True)
                if distance > 0:
                    substring_text = string.unsafe(str_ncpy).decode()
                    print(" " * (distance - 1), f"{substring_text}[{distance}]")
            current_ptr = match_ptr

    stack.free(stack_ptr2)

if __name__ == "__main__":
    main()
