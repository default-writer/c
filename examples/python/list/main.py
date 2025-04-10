# examples/python/list/main.py
import ctypes
from c import CVirtualMachine, CList, CException


def main():
    try:
        try:
            # Create a Virtual Machine instance with a size of 8.
            with CVirtualMachine(8) as cvm_ptr:
                with CList(cvm_ptr) as list_ptr:

                    # Push some data onto the list
                    list_ptr.push(1)
                    list_ptr.push(2)
                    list_ptr.push(3)
                    list_ptr.push(4)
                    list_ptr.push(5)

                    while (memory_ptr := list_ptr.pop()):
                        print(f"{memory_ptr:016x}")


        except CException as e:
            # Catch any exceptions that occur during the process and print an error message.
            print(f"error: {e}")

    except Exception as e:
        # Catch any exceptions that occur during the process and print an error message.
        print(f"error: {e}")

if __name__ == "__main__":
    main()
