# debugging using GDB

## troubleshooting segmentation fault in your C extensions

to run the Python test under `gdb` for debugging, you need to attach `gdb` to the Python interpreter that runs the test. here's how you can do it:

```bash
gdb --args python3 -m pytest examples/python/tests/cstack/test_cstack.py
```

---

## steps to Run the Test Under `gdb`

1. **start `gdb` with Python**:

   run `gdb` and specify the Python interpreter as the executable. for example:

   ```bash
   gdb --args python3 -m pytest examples/python/tests/cstack/test_cstack.py
   ```

   - `--args`: allows you to pass arguments to the Python interpreter.
   - `python3`: the Python interpreter.
   - `-m pytest`: runs the `pytest` module.
   - test_cstack.py: the specific test file to run.

2. **set a breakpoint (optional)**:

   if you want to debug a specific function in your C extension, set a breakpoint in `gdb` before running the test. for example:

   ```bash
   (gdb) break stack_peek
   ```

    gdb may ask you to set a breakpoint on not yet loaded modules, your answer is `yes` (y)

3. **run the test**:

   start the test execution in `gdb`:

   ```bash
   (gdb) run
   ```

4. **analyze the crash**:

   if a segmentation fault (`SIGSEGV`) occurs, `gdb` will pause execution and show the location of the crash.

   - **backtrace**:

     ```bash
     (gdb) backtrace
     ```

     this shows the call stack leading to the crash.

   - **inspect variables**:

     use `print` to inspect variables. For example:

     ```bash
     (gdb) print address
     ```

   - **step through code**:

     use `step` (s), `next` (n) or 'continue' (c) to step through the code line by line.

5. **exit `gdb`**:

   once you have finished debugging, you can exit `gdb`:

   ```bash
   (gdb) quit
   ```

---

## example workflow

```bash
$ gdb --args python3 -m pytest examples/python/tests/cstack/test_cstack.py
(gdb) break stack_peek
(gdb) run
(gdb) backtrace
(gdb) print address
(gdb) quit
```

---

## notes

- ensure that your C extension is compiled with debugging symbols (`-g` flag). If you are using `CMake`, add the following to your CMakeLists.txt:

  ```cmake
  set(CMAKE_BUILD_TYPE Debug)
  ```

- if the crash occurs in a specific test function (e.g., `test_stack_peek_0`), you can run only that test:

  ```bash
  gdb --args python3 -m pytest examples/python/tests/cstack/test_cstack.py::test_stack_peek_0
  ```
