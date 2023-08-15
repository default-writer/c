# c project description

- virtual machine implementation on C
    1. C API
    2. memory management
    3. garbage collection

- development environment:

    1. WSL2/devcontainers
    2. Ubuntu 22.04.3 LTS

![ubuntu logo](./resources/icons8-ubuntu-96.png)

![logo](./resources/logo-tiny.png)

## media links

- streaming channels

    1. [twitch.tv](https://www.twitch.tv/defaultwriter/videos)
    2. [youtube.com](https://www.youtube.com/channel/UCrtPCqirYXdA2gYG8NxbTjA)

## updates

- 2023

    1. added v1 core Virtual Machine (vm1) with types:
    2. added support including:

        1. file IO
        2. string manupulation methods
        3. search

## features

- `List<T>` implementations
- code coverage
- coverage hot-reload
- memory allocation playground
- automatic garbage collection implementation (GC)
- GTK GUI app demo ready to run on containerized environment

## readme

repo is ready to run on containers, WSL, remote hosts.

### remote development

run (as co-owner or contributor)

- eval `ssh-agent -s`
- git config --global user.name "$USER_NAME"
- git config --global user.email "$USER_EMAIL"
- git clone https://github.com/default-writer/c.git

### containers

run

- ./bin/init.sh
 
## GTK GUI app development:

run

- ./bin/gtkbuild.sh --target gtk-experiment --clean
- UI_MODE=--ui ./gtk/gtk-experiment

## shell scripting documentation

- all shell (mostly bash) scripts located in ./bin folder
- all shell scripts from ./bin and ./bin/utils/ can be used and run as self-documented scripts, with embedded help
- all shell scripts knows how to execute as user or root (sudo)
- all shell scripts have required command-line options and optional [] parameters

## history

### 2023

- added GC implementation
- added several list implementations
  
### 2022

- playing around with visibility in class declarations

## documentation

- [memory](docs/memory.md) PDF([memory.pdf][memory])
- [list](docs/list.md) PDF([list.pdf][list])
- [class](docs/class.md) PDF([class.pdf][class])
- [history](docs/history.md) PDF([history][history])
- [scripts](docs/scripts.md) PDF([scripts][scripts])
- [labs](docs/labs.md) PDF([labs][labs])

## resources

- [ubuntu](https://icons8.com/icon/63208/ubuntu") icon by [icons8](https://icons8.com)

## links

- [git](https://git-scm.com)
- [pyenv](https://github.com/pyenv/pyenv)
- [python](https://www.python.org)
- [cmake](https://cmake.org)
- [docker](https://docker.com)
- [cli](https://github.com/cli/cli/blob/trunk/docs/install_linux.md)
- [hooks](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks)

## requirements

- [rexo](https://github.com/christophercrouzet/rexo)

[memory]: docs/pdf/memory.pdf
[list]: docs/pdf/list.pdf
[class]: docs/pdf/class.pdf
[history]: docs/pdf/history.pdf
[scripts]: docs/pdf/scripts.pdf
[labs]: docs/pdf/labs.pdf
