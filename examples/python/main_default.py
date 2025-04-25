import traceback
from c import CVirtualMachine, CString, CException


def main():
    try:
        with CVirtualMachine(8) as vm:
            with CString(vm, b"a".decode()) as cstring1:
                result = CString.copy(vm, cstring1.ptr())
                print(f"result: {result}")
    except CException as e:
        traceback.print_exc()
        print(f"error: {e}")
    except Exception as e:
        traceback.print_exc()
        print(f"error: {e}")


if __name__ == "__main__":
    main()
