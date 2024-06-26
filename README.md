# c project description

<a href="" target="blank"><img style='height:128px' src='./resources/ecommerce.png'></a>

## status

[![C/C++ CI](https://github.com/default-writer/c/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/default-writer/c/actions/workflows/c-cpp.yml)
[![Coverage Status](https://coveralls.io/repos/github/default-writer/c/badge.svg?branch=main)](https://coveralls.io/github/default-writer/c?branch=main)

## description

- C memory management

```c
static u64 read_data(u64 list_ptr, const char* prompt) {
    u64 data_ptr = 0;
    u64 ui_mode_ptr = string->load("UI_MODE");
    u64 mode_ptr = string->load("--ui");
    u64 value_ptr = os->getenv(ui_mode_ptr);
    if (ui_mode_ptr != 0 && string->strcmp(value_ptr, mode_ptr) != 0) {
        data_ptr = read_input(prompt);
    } else {
        printf(">%s:\n", prompt);
        data_ptr = list->pop(list_ptr);
        if (data_ptr == 0) {
            printf("\n");
        }
    }
    return data_ptr;
}
```

```c
int main(void) {
    pointer->init(8); // initializes virtual memory manager with chunk size of 8

    // some work with standard classes: os, string, data, etc.

    pointer->gc();      // releases all resources added to GC
    pointer->destroy(); // releases virtual memory manager
    return 0;
}
```

## inspiration

- [tilir](https://github.com/tilir/c-graduate)
- [introduction to C (ru)](https://www.youtube.com/watch?v=7YhRFx-oyW4&ab_channel=KonstantinVladimirov)
- [practice C bash/linux/gcc (ru)](https://youtu.be/KLv-5jWlKjM)

## vision

- managed (C programming language model)
- safe
- friendly
- extensibe
- simple
- tested (100% code coverage)

## brief

<a href="" target="blank"><img style='height:128px' src='./resources/api.png'></a>

- VM managed types / C API

- user defined types / extensibility system

  1. C API
  2. memory management
  3. garbage collection

- development environment:

  1. WSL2/devcontainers
  2. Ubuntu 22.04.3 LTS

- C API generated docs (doxygen):

  1. [docs](./docs/refman.pdf)

![logo](./resources/logo-tiny.png)

## media links

<a href="" target="blank"><img style='height:128px' src='./resources/data-science.png'></a>

- streaming channels

  1. [twitch.tv](https://www.twitch.tv/defaultwriter/videos)
  2. [youtube.com](https://www.youtube.com/channel/UCrtPCqirYXdA2gYG8NxbTjA)

## music credits

- [imperss](https://soundcloud.com/imperss)

- [Adrian von Ziegler](https://soundcloud.com/adrian-von-ziegler)

## updates

<a href="" target="blank"><img style='height:128px' src='./resources/checklist.png'></a>

- 2023

  1. added v1 core Virtual Machine (vm1) with types:
  2. added support including:

     1. file IO
     2. string manupulation methods
     3. search

## features

<a href="" target="blank"><img style='height:128px' src='./resources/global-distribution.png'></a>

- `List<T>` implementations
- code coverage
- coverage hot-reload
- memory allocation playground
- automatic garbage collection implementation (GC)
- GTK GUI app demo ready to run on containerized environment

## readme

<a href="" target="blank"><img style='height:128px' src='./resources/digital-library.png'></a>

repo is ready to run on containers, WSL, remote hosts.

### remote development

- installation from git repo (manually)

Installs sources from git [repo](https://github.com/default-writer/c.git)

```bash
eval `ssh-agent -s`
git config --global user.name "$USER_NAME"
git config --global user.email "$USER_EMAIL"
git clone https://github.com/default-writer/c.git
```

## clang

[docs](https://manpages.ubuntu.com/manpages/jammy/man1/clangd-11.1.html)

## containers

<a href="" target="blank"><img style='height:128px' src='./resources/digital-library.png'></a>

### manual installation (minimal install)

- init

Sets up primary/optional tools and git hooks

```bash
./bin/init.sh --setup
./bin/init.sh --optional
./bin/init.sh --hooks
```

- rexo

```bash
./bin/utils/install.sh --submodule-rexo
```

## GTK GUI app development

<a href="" target="blank"><img style='height:128px' src='./resources/digital-library.png'></a>

### setup

- add/update Visual Studio Code environment settings:

```json
"environment": [
  {
    "name": "UI_MODE",
    "value": "--ui"
  }
]
```

- run command line build

```bash
cd gtk
./bin/build.sh --target gtk-experiment1 --clean
UI_MODE=--ui ./build/gtk-experiment1
```

## shell scripting documentation

### all shell scripts

- are self-documented
- placed in ./bin folders
- detects execution as user or root (sudo)
- runs with required and optional command-line parameters

## history

<a href="" target="blank"><img style='height:128px' src='./resources/digital-library.png'></a>

## local repos

Use [CLI](https://github.com/default-writer/tree/master/cli) or [git](https://github.com/default-writer/c.git).

## support C project

### sponsorship

support this project [become a sponsor](https://opencollective.com/c-vm#sponsor)

[![img](https://opencollective.com/default-writer/sponsor/0/avatar.svg)](https://opencollective.com/c-vm/sponsor/0/website)

### 2023

- added support for classes / inheritance (oberon exaple)
- added GC implementation
- added several list implementations

### 2022

- playing around with visibility in class declarations

## documentation

### initialization

```bash
- ./bin/init.sh --setup
- ./bin/init.sh --hooks
- ./bin/init.sh --optional
```

## doxygen generated documentation

<a href="" target="blank"><img style='height:128px' src='./resources/digital-library.png'></a>

[C API](docs/refman.pdf) PDF([refman.pdf][refman])

### referential documentation

- [memory](docs/memory.md) PDF([memory.pdf][memory])
- [list](docs/list.md) PDF([list.pdf][list])
- [class](docs/class.md) PDF([class.pdf][class])
- [history](docs/history.md) PDF([history][history])
- [scripts](docs/scripts.md) PDF([scripts][scripts])
- [labs](docs/labs.md) PDF([labs][labs])

## resources

<a href="" target="blank"><img style='height:128px' src='./resources/digital-library.png'></a>

- [ubuntu](https://icons8.com/icon/63208/ubuntu") icon by [icons8](https://icons8.com)
- [vectorsmarket15](https://www.flaticon.com/authors/vectorsmarket15)
- [ecommerce icons](https://www.flaticon.com/free-icons/ecommerce)
- [flaticon](https://www.flaticon.com)

## links

<a href="" target="blank"><img style='height:128px' src='./resources/digital-library.png'></a>

- [git](https://git-scm.com)
- [pyenv](https://github.com/pyenv/pyenv)
- [python](https://www.python.org)
- [cmake](https://cmake.org)
- [docker](https://docker.com)
- [cli](https://github.com/cli/cli/blob/trunk/docs/install_linux.md)
- [hooks](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks)

## requirements

<a href="" target="blank"><img style='height:128px' src='./resources/digital-library.png'></a>

- [rexo](https://github.com/christophercrouzet/rexo)

## SAST Tools

[PVS-Studio](https://pvs-studio.com/pvs-studio/?utm_source=website&utm_medium=github&utm_campaign=open_source) - static analyzer for C, C++, C#, and Java code.

[refman]: docs/refman.pdf
[memory]: docs/pdf/memory.pdf
[list]: docs/pdf/list.pdf
[class]: docs/pdf/class.pdf
[history]: docs/pdf/history.pdf
[scripts]: docs/pdf/scripts.pdf
[labs]: docs/pdf/labs.pdf
