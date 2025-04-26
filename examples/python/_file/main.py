import ctypes
import traceback
from c import CException, COperatingSystem, CVirtualMachine, CString, CFile, CData


def main():
    try:
        try:

            with CVirtualMachine(8) as vm:

                cstring: CString = CString(vm)
                cfile: CFile = CFile(vm)
                cdata: CData = CData(vm)

                path_ptr = cstring.load(str(__file__).decode())
                mode_ptr = cstring.load(b"r".decode())
                file_ptr = cfile.alloc(path_ptr, mode_ptr)

                cstring.free(path_ptr)
                cstring.free(mode_ptr)

                data_ptr = cfile.data(file_ptr)

                cfile.free(file_ptr)

                file_data_ptr = cdata.unsafe(data_ptr)

                COperatingSystem.puts(ctypes.cast(file_data_ptr, ctypes.c_char_p))

                cdata.free(data_ptr)

        except CException as e:
            traceback.print_exc()
            print(f"error: {e}")

    except Exception as e:
        traceback.print_exc()
        print(f"error: {e}")


if __name__ == "__main__":

    main()
