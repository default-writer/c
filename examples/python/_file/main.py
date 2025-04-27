import traceback
from c import CException, CEnvironment, CVirtualMachine, CString, CFile, CData


def main():
    try:
        try:

            with CVirtualMachine(8) as vm:

                path = CString(vm, str(__file__))
                mode = CString(vm, b"r".decode())
                file: CFile = CFile(vm, path.ptr(), mode.ptr())
                CString.free(vm, path.ptr())
                CString.free(vm, mode.ptr())

                data_ptr = file.data()
                CFile.free(vm, file.ptr())
                text: CString = CString(vm, CData.unsafe(vm, data_ptr))
                CData.free(vm, data_ptr)

                CEnvironment.puts(vm, text.ptr())
                CString.free(vm, text.ptr())

        except CException as e:
            traceback.print_exc()
            print(f"error: {e}")

    except Exception as e:
        traceback.print_exc()
        print(f"error: {e}")


if __name__ == "__main__":

    main()
