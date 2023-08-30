# c project description

<a href="" target="blank"><img style='height:128px' src='./resources/microprocessor.png'></a>


## vision


- C project targets to extend vision of C programming language as the C programming language library powered with API without complex pointer arithmetic, pointers, safe to use and friendly for programming new modern applications.


- virtual machine implementation on C

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

run (as co-owner or contributor)

- eval `ssh-agent -s`
- git config --global user.name "$USER_NAME"
- git config --global user.email "$USER_EMAIL"
- git clone [https://github.com/default-writer/c.git](https://github.com/default-writer/c.git)

### containers

run

- ./bin/init.sh

## GTK GUI app development

```bash
cd gtk
./bin/build.sh --target gtk-experiment1 --clean
UI_MODE=--ui ./build/gtk-experiment1
```

## shell scripting documentation

- all shell scripts located in ./bin folder
- all shell scripts from ./bin and ./bin/utils/ can be used and run as self-documented scripts, with embedded help
- all shell scripts correctly detectes execution as user or root (sudo)
- all shell scripts have required command-line options and optional [] parameters

## history

## local repos

You can use c project for local git repos with the [CLI](https://github.com/default-writer/tree/master/cli) or [git](https://github.com/default-writer/c.git).

## support C project

### sponsorship

support this project [become a sponsor](https://opencollective.com/c-vmr#sponsor)

[![img](https://opencollective.com/default-writer/sponsor/0/avatar.svg)](https://opencollective.com/c-vm/sponsor/0/website)


### 2023

- added support for classes / inheritance (oberon exaple)
- added GC implementation
- added several list implementations

### 2022

- playing around with visibility in class declarations

## documentation

### doxygen generated documentation

[C API](docs/refman.pdf) PDF([refman.pdf][refman])

### referential documentation

- [memory](docs/memory.md) PDF([memory.pdf][memory])
- [list](docs/list.md) PDF([list.pdf][list])
- [class](docs/class.md) PDF([class.pdf][class])
- [history](docs/history.md) PDF([history][history])
- [scripts](docs/scripts.md) PDF([scripts][scripts])
- [labs](docs/labs.md) PDF([labs][labs])

## resources

- [ubuntu](https://icons8.com/icon/63208/ubuntu") icon by [icons8](https://icons8.com)
- [vectorsmarket15](https://www.flaticon.com/authors/vectorsmarket15)
- [ecommerce icons](https://www.flaticon.com/free-icons/ecommerce)
- [flaticon](https://www.flaticon.com)

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

## gists

- [cpu frequency cotrol (widows)](https://gist.github.com/default-writer/7257a734def25a2f3b6c636c8fb463d7)

[refman]: docs/refman.pdf
[memory]: docs/pdf/memory.pdf
[list]: docs/pdf/list.pdf
[class]: docs/pdf/class.pdf
[history]: docs/pdf/history.pdf
[scripts]: docs/pdf/scripts.pdf
[labs]: docs/pdf/labs.pdf
