# TODO: implement sample java class to compile and build using javac

```.gitignore
*.class
```

```clangd
CompileFlags:
  Add:
    - "-Wno-undefined-inline"
    - "-I/usr/lib/jvm/default-java/include/linux"
    - "-I/usr/lib/jvm/default-java/include"
    - "-I/usr/include/gtk-4.0"
    - "-I.deps/rexo/include"
    - "-Iexamples/java/include"
    - "-Iexamples/java/src"
    - "-Isrc"
  Compiler: clang
```

```CmakeLists.txt
include_directories(${JAVA_HOME}/include/linux)
include_directories(${JAVA_HOME}/include)
include_directories(/usr/include/gtk-4.0)
include_directories(${CMAKE_SOURCE_DIR}/.deps/rexo/include)
include_directories(${CMAKE_SOURCE_DIR}/examples/java/include)
include_directories(${CMAKE_SOURCE_DIR}/examples/java/src)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/tests)
```

```compile_filags.txt
-I/usr/lib/jvm/default-java/include/linux
-I/usr/lib/jvm/default-java/include
-I/usr/include/gtk-4.0
-I.deps/rexo/include
-Iexamples/java/include
-Iexamples/java/src
-Isrc
-DCONFIG_MEMORY_CLEANUP
-DCONFIG_MEMORY_DEBUG_INFO
-DCONFIG_GC
```

```bash
#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

err_report() {
    cd "${source}"
    echo "ERROR: $0:$*"
    exit 8
}

if [[ "${BASHOPTS}" != *extdebug* ]]; then
    trap 'err_report $LINENO' ERR
fi

source=$(pwd)

pwd=$(cd "$(dirname $(dirname "${BASH_SOURCE[0]}"))" &> /dev/null && pwd)

cd "${pwd}"

install="$1"

opts=( "${@:2}" )

. "${pwd}/bin/scripts/load.sh"


## Builds java sources
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--help") # [optional] shows command description
            help
            ;;

        "--java") # [optional] extracts javac library info

            echo "Compiling Java files, generating JNI headers, and building shared libraries..."
            cd "${pwd}/examples/java"

            # Compile all Java files
            javac -cp . *.java

            # Create the include and lib directories if they don't exist
            mkdir -p include
            mkdir -p lib

            # Generate JNI header files for each class
            javac -h include -cp . *.java

            export JAVA_HOME=/usr/lib/jvm/java-21-openjdk-amd64
            # Compile libc-sys.so
            # echo "Compiling libc-sys.so..."
            gcc -c -fPIC \
                ${pwd}/examples/java/src/c-sys.c \
                -I${JAVA_HOME}/include/linux \
                -I${JAVA_HOME}/include/ \
                -I${pwd}/src \
                -o ${pwd}/examples/java/lib/libc-sys.o
            gcc -shared \
                ${pwd}/examples/java/lib/libc-sys.o \
                -Wl,-rpath,L${pwd}/lib \
                -L${pwd}/lib \
                -o ${pwd}/examples/java/lib/libc-sys.so

            # Compile libc-vm.so
            # echo "Compiling libc-vm.so..."
            gcc -c -fPIC \
                ${pwd}/examples/java/src/c-vm.c \
                -I${JAVA_HOME}/include/linux \
                -I${JAVA_HOME}/include/ \
                -I${pwd}/src \
                -o ${pwd}/examples/java/lib/libc-vm.o
            gcc -shared \
                ${pwd}/examples/java/lib/libc-sys.o ${pwd}/examples/java/lib/libc-vm.o \
                -Wl,-rpath,L${pwd}/lib \
                -L${pwd}/lib \
                -lc-sys \
                -o ${pwd}/examples/java/lib/libc-vm.so

            echo "JNI header files generated in the 'include' directory."
            ;;

        *)
            help
            ;;

    esac
    shift
done

if [[ "${install}" == "" ]]; then
    help
    exit
fi


[[ $SHLVL -eq 2 ]] && echo OK

cd "${pwd}"

```
