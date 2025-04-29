import time
import traceback
from c import CList, CList2


def main():
    try:
        start_time = time.perf_counter_ns()
        _list = []
        for i in range(1000000):
            _list.append(1)
        end_time = time.perf_counter_ns()
        elapsed_ns = end_time - start_time
        del _list

        list_v1_ptr: CList = CList()
        start_time1 = time.perf_counter_ns()
        for i in range(1000000):
            list_v1_ptr.push(1)
        end_time1 = time.perf_counter_ns()
        elapsed_ns1 = end_time1 - start_time1
        del list_v1_ptr

        list_v2_ptr: CList2 = CList2(1000000)
        start_time2 = time.perf_counter_ns()
        for i in range(1000000):
            list_v2_ptr.push(1)
        end_time2 = time.perf_counter_ns()
        elapsed_ns2 = end_time2 - start_time2
        del list_v2_ptr

        print_elapsed("  list", elapsed_ns)
        print_elapsed(" clist", elapsed_ns1)
        print_elapsed("clist2", elapsed_ns2)

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
