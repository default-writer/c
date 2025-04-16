# examples/python/list/main.py
import time  # Import the time module for timing
from c import CException
from c import CVirtualMachine
from c import CList

def main():
    try:
        with CVirtualMachine(1000000) as cvm_ptr:
            with CList(cvm_ptr) as list_ptr:

                try:
                    list_ptr.push(0)
                except CException as e:
                    print(f"error: {e}")

                start_time = time.perf_counter_ns()
                for i in range(1000000):
                    list_ptr.push(1)
                end_time = time.perf_counter_ns()

                # Calculate elapsed time in nanoseconds
                elapsed_ns = end_time - start_time

                # Convert elapsed time to hh:mm:ss.ms.ns format
                hours = elapsed_ns // (1_000_000_000 * 60 * 60)
                elapsed_ns %= (1_000_000_000 * 60 * 60)
                minutes = elapsed_ns // (1_000_000_000 * 60)
                elapsed_ns %= (1_000_000_000 * 60)
                seconds = elapsed_ns // 1_000_000_000
                milliseconds = (elapsed_ns % 1_000_000_000) // 1_000_000
                nanoseconds = elapsed_ns % 1_000_000

                # Print the timing in the desired format
                print(f"{hours:02}:{minutes:02}:{seconds:02}.{milliseconds:03}.{nanoseconds:06}")

    except Exception as e:
        print(f"error: {e}")

if __name__ == "__main__":
    main()
