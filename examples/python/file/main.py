# examples/python/file/main.py
import ctypes

from c import CVirtualMachine, CString, CFile, CData, COperatingSystem, CException


def main():
    try:
        try:
            # Create a Virtual Machine instance with a size of 8.
            with CVirtualMachine(8) as cvm_ptr:

                # Create instances of CString, CFile, and CData, associating them with the VM.
                cstring: CString = CString(cvm_ptr)
                cfile: CFile = CFile(cvm_ptr)
                cdata: CData = CData(cvm_ptr)

                # Load the current file path as a string into the VM.
                path_ptr = cstring.load(str(__file__).encode())
                # Load the file mode "r" (read) as a string into the VM.
                mode_ptr = cstring.load(b"r")

                # Allocate a file in the VM using the loaded path and mode.
                file_ptr = cfile.alloc(path_ptr, mode_ptr)

                # Free the path and mode strings as they are no longer needed.
                cstring.free(path_ptr)
                cstring.free(mode_ptr)

                # Get the data associated with the allocated file.
                data_ptr = cfile.data(file_ptr)
                # Free the file resource.
                cfile.free(file_ptr)

                # Get an unsafe pointer to the file data.
                file_data_ptr = cdata.unsafe(data_ptr)

                # Print the file data to standard output using COperatingSystem.puts.
                COperatingSystem.puts(ctypes.cast(file_data_ptr, ctypes.c_char_p))

                # Free the data resource.
                cdata.free(data_ptr)

        except CException as e:
            # Catch any exceptions that occur during the process and print an error message.
            print(f"error: {e}")

    except Exception as e:
        # Catch any exceptions that occur during the process and print an error message.
        print(f"error: {e}")


if __name__ == "__main__":
    # Execute the main function if the script is run directly.
    main()
