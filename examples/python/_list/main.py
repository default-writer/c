import time
import traceback
from c import CVirtualMachine, CList


def main():
    try:
        with CVirtualMachine(1000000) as vm:

            start_time1 = time.perf_counter_ns()
            _list = []
            for i in range(1000000):
                _list.append(1)
            end_time1 = time.perf_counter_ns()
            elapsed_ns1 = end_time1 - start_time1
            del _list

            list_ptr: CList = CList()
            start_time2 = time.perf_counter_ns()
            for i in range(1000000):
                list_ptr.push(1)
            end_time2 = time.perf_counter_ns()
            elapsed_ns2 = end_time2 - start_time2
            del list_ptr

            print_elapsed(" list", elapsed_ns1)
            print_elapsed("clist", elapsed_ns2)

    except Exception as e:
        traceback.print_exc()
        print(f"error: {e}")


def print_elapsed(message, elapsed_ns):
    hours = elapsed_ns // (1_000_000_000 * 60 * 60)
    elapsed_ns %= 1_000_000_000 * 60 * 60
    minutes = elapsed_ns // (1_000_000_000 * 60)
    elapsed_ns %= 1_000_000_000 * 60
    seconds = elapsed_ns // 1_000_000_000
    milliseconds = (elapsed_ns % 1_000_000_000) // 1_000_000
    nanoseconds = elapsed_ns % 1_000_000

    print(f"{message}: {hours:02}:{minutes:02}:{seconds:02}.{milliseconds:03}.{nanoseconds:06}")


if __name__ == "__main__":
    main()
