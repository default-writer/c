# examples/python/list/main.py
import ctypes

from c import CVirtualMachine, CList, CException


def main():
    try:
        try:
            # Create a Virtual Machine instance with a size of 8.
            with CVirtualMachine(8) as cvm_ptr:

                clist: CList = CList(cvm_ptr)
                list_ptr = clist.init()

                # Push some data onto the list
                clist.push(list_ptr, 1)
                clist.push(list_ptr, 2)
                clist.push(list_ptr, 3)
                clist.push(list_ptr, 4)
                clist.push(list_ptr, 5)

                while (memory_ptr := clist.pop(list_ptr)):
                    print(f"{memory_ptr:016x}")

                clist.destroy(list_ptr)

        except CException as e:
            # Catch any exceptions that occur during the process and print an error message.
            print(f"error: {e}")

    except Exception as e:
        # Catch any exceptions that occur during the process and print an error message.
        print(f"error: {e}")

if __name__ == "__main__":
    main()
