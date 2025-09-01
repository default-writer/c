#!/usr/bin/env bash
export LD_PRELOAD="$PWD/lib/libc-sys.so:$PWD/lib/libc-vm.so"
export LD_LIBRARY_PATH="$PWD/c/lib"
