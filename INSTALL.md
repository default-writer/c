# Installation on Windows (MSYS2/CMAKE)

- Install [MSys2](https://www.msys2.org/docs/cmake/)

- Install [Node.js](https://nodejs.org/en/download/prebuilt-installer)

- Install  packages `cmake` `ninja` `clang` (MSYS2)

```bash
pacman -S --noconfirm \
    mingw-w64-ucrt-x86_64-cmake \
    mingw-w64-ucrt-x86_64-gcc \
    mingw-w64-ucrt-x86_64-ninja \
    mingw-w64-ucrt-x86_64-cmake \
    mingw-w64-ucrt-x86_64-lcov \
    mingw-w64-clang-x86_64-cmake \
    mingw-w64-clang-x86_64-gcc-compat \
    mingw-w64-clang-x86_64-ninja \
    mingw-w64-clang-x86_64-cmake \
    mingw-w64-clang-x86_64-lcov \
    cmake ninja clang lcov git
```

- Add paths UCRT binaries/libraries at the top to the *system* variable (PATH)

```cmd
set PATH="C:\msys64\ucrt64;C:\msys64\ucrt64\bin;C:\msys64\ucrt64\include;C:\msys64\ucrt64\lib;%PATH%"
```
