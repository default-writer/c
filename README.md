# c project description

![image info](./resources/icons8-ubuntu-96.png)

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
