# Installation on Windows (MSYS2/CMAKE)

- Install [MSys2](https://www.msys2.org/docs/cmake/)

- (optionally) Install [Node.js](https://nodejs.org/en/download/prebuilt-installer)

- Install  packages using msys shell (MSYS2)

```bash
pacman -S --noconfirm \
    mingw-w64-ucrt-x86_64-cmake \
    mingw-w64-ucrt-x86_64-gcc \
    mingw-w64-ucrt-x86_64-ninja \
    mingw-w64-ucrt-x86_64-cmake \
    mingw-w64-ucrt-x86_64-lcov \
    mingw-w64-ucrt-x86_64-gdb \
    mingw-w64-clang-x86_64-cmake \
    mingw-w64-clang-x86_64-gcc-compat \
    mingw-w64-clang-x86_64-ninja \
    mingw-w64-clang-x86_64-cmake \
    mingw-w64-clang-x86_64-lcov \
    mingw-w64-clang-x86_64-gdb \
    cmake ninja clang lcov gdb
```

- Add paths to UCRT binaries/libraries (C:\msys64\ucrt64\bin) at the top to the *system* variable (PATH)

    - Add to the top of PATH environment variables value `C:\msys64\clang64\bin` (clang64)

    or

    - Add to the top of PATH environment variables value `C:\msys64\clang64\bin` (ucrt64)

- Run ./bin/init.sh --init (MSYS2)

```cmd
set PATH="C:\msys64\ucrt64;C:\msys64\ucrt64\bin;C:\msys64\ucrt64\include;C:\msys64\ucrt64\lib;%PATH%"
```
