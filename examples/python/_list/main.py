import time
import traceback
from c import CException, CVirtualMachine, CList


def main():
    try:
        with CVirtualMachine(1000000) as vm:
            list_ptr: CList = CList()
            try:
                list_ptr.push(0)
            except CException as e:
                traceback.print_exc()
                print(f"error: {e}")

            start_time = time.perf_counter_ns()
            for i in range(1000000):
                list_ptr.push(1)
            end_time = time.perf_counter_ns()

            elapsed_ns = end_time - start_time

            hours = elapsed_ns // (1_000_000_000 * 60 * 60)
            elapsed_ns %= 1_000_000_000 * 60 * 60
            minutes = elapsed_ns // (1_000_000_000 * 60)
            elapsed_ns %= 1_000_000_000 * 60
            seconds = elapsed_ns // 1_000_000_000
            milliseconds = (elapsed_ns % 1_000_000_000) // 1_000_000
            nanoseconds = elapsed_ns % 1_000_000

            del list_ptr
            print(f"{hours:02}:{minutes:02}:{seconds:02}.{milliseconds:03}.{nanoseconds:06}")

    except Exception as e:
        traceback.print_exc()
        print(f"error: {e}")


if __name__ == "__main__":
    main()
