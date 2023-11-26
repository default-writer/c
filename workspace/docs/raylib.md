# raylib

[link](https://www.raylib.com)

## windows

### WSL2 development error

[/usr/lib/wsl/lib/libcuda.so.1 is not a symbolic link](https://github.com/microsoft/WSL/issues/5548#issuecomment-912495487)

1. Open cmd as Administrator and cd into C:\Windows\System32\lxss\lib
2. Delete libcuda.so and libcuda.so.1 (You can also do this in Windows Explorer as well)
3. Run wsl -e /bin/bash in cmd and you should already in /mnt/c/Windows/System32/lxss/lib, now you have permission to create symlink:

```bash
    ln -s libcuda.so.1.1 libcuda.so
    ln -s libcuda.so.1.1 libcuda.so.1
```
