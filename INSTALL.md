# installation on Windows (MSYS2)

- install MSYS2 using CLI

[MSYS2 CLI](https://repo.msys2.org/distrib/) for `msys2-x86_64-latest`

## install MSYS2

- add path at the top to the *system* variables (PATH) i.e. add `C:\tools\install` (C:\tools\install\msys2-x86_64-latest.exe)

```bash
msys2-x86_64-latest in --confirm-command --accept-messages --root C:/msys64
```

### start MSYS2 shell (CLANG64)

```bash
C:\msys64\clang64.exe
```

- install  packages (MSYS2 shell)

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
    mingw-w64-clang-x86_64-clang-tools-extra \
    mingw-w64-ucrt-x86_64-clang-tools-extra \
    cmake ninja clang lcov gdb git man
```

### select default VSCode profile

- add `bash (MSYS2)` as default profile i.e. `C:\msys64\usr\bin\bash.exe`

## OPTIONAL

- install [nodejs](https://nodejs.org/en/download/prebuilt-installer)

### UCRT64

- add path at the top to the *system* variables (PATH) `C:\msys64\ucrt64\bin` (ucrt64)

### CLANG64

- add path at the top to the *system* variables (PATH) `C:\msys64\clang64\bin` (clang64)

- run ./bin/init.sh --init (MSYS2)

```cmd
set PATH="C:\msys64\ucrt64;C:\msys64\ucrt64\bin;C:\msys64\ucrt64\include;C:\msys64\ucrt64\lib;%PATH%"
```
