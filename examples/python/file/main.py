# examples/python/file/main.py
import ctypes

from c import CVirtualMachine, CString, CFile, CData, COperatingSystem, CException


def main():
    try:
        try:
            # Create a Virtual Machine instance with a size of 8.
            cvm_ptr: CVirtualMachine = CVirtualMachine(8)

            # Create instances of CString, CFile, and CData, associating them with the VM.
            s: CString = CString(cvm_ptr)
            f: CFile = CFile(cvm_ptr)
            d: CData = CData(cvm_ptr)

            # Load the current file path as a string into the VM.
            path_ptr = s.load(str(__file__).encode())
            # Load the file mode "r" (read) as a string into the VM.
            mode_ptr = s.load(b"r")

            # Allocate a file in the VM using the loaded path and mode.
            file_ptr = f.alloc(path_ptr, mode_ptr)

            # Free the path and mode strings as they are no longer needed.
            s.free(path_ptr)
            s.free(mode_ptr)

            # Get the data associated with the allocated file.
            data_ptr = f.data(file_ptr)
            # Free the file resource.
            f.free(file_ptr)

            # Get an unsafe pointer to the file data.
            file_data_ptr = d.unsafe(data_ptr)

            # Print the file data to standard output using COperatingSystem.puts.
            COperatingSystem.puts(ctypes.cast(file_data_ptr, ctypes.c_char_p))

            # Free the data resource.
            d.free(data_ptr)

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


if __name__ == "__main__":
    # Execute the main function if the script is run directly.
    main()
