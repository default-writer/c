from c import CVirtualMachine, CString, CFile, CData, COperatingSystem
import ctypes


def main():
    try:
        cvm_ptr: CVirtualMachine = CVirtualMachine(8)
        s: CString = CString(cvm_ptr)
        f: CFile = CFile(cvm_ptr)
        d: CData = CData(cvm_ptr)
        path_ptr = s.load(str(__file__).encode())
        mode_ptr = s.load(b"r")
        file_ptr = f.alloc(path_ptr, mode_ptr)
        s.free(path_ptr)
        s.free(mode_ptr)
        data_ptr = f.data(file_ptr)
        f.free(file_ptr)
        file_data_ptr = d.unsafe(data_ptr)
        COperatingSystem.puts(ctypes.cast(file_data_ptr, ctypes.c_char_p))
        d.free(data_ptr)
        cvm_ptr.gc()
        del cvm_ptr
    except Exception as e:
        print(f"error: {e}")


if __name__ == "__main__":
    main()
