# Description

- C List<T> implementation

Links: 
https://code.visualstudio.com/docs/cpp/config-mingw
https://www.msys2.org/


Installation instructions:

https://stackoverflow.com/questions/67273235/using-ninja-as-a-generator-with-msys2-mingw-on-windows-10-x64


- setx PATH C:\msys64\mingw64\bin;C:\msys64;%PATH%;
- pacman -S cmake -y
- pacman -S ninja -y
- pacman -S lcov -y
- pacman -S --needed base-devel mingw-w64-x86_64-toolchain -y
- pacman -S mingw-w64-x86_64-cmake -y
- pacman -S mingw-w64-x86_64-ninja -y
- pacman -S mingw-w64-x86_64-lcov -y
