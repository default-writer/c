# c project description

- `List<T>` implementations
- code coverage
- coverage hot-reload

# timeline

## 2023

- added implementation without realloc with random access inside a block

## 2022

- playing around with visibility in class declarations


# lab

- working with containers requires some in-place installations and batch scripting both in host and inside the running development container
- when created container for the first time it is strongly recommended to use scripts to cleanup and install dependencies


#  shell

### bootstraps build

```sh
./bin/runme.sh
```

## install

### installs optional components:

```sh
./bin/install.sh
```

## uninstall

### uninstalls optional components:

```sh
./bin/uninstall.sh
```

## cleanup

### cleans up the root directory

- forsefully removes any generated folders
- breaks sonarlint and other directory based stuff (sonarlint searchs for **build** directory)

```sh
./bin/cleanup.sh
```

## coverage

### builds sources with code coverage

```sh
./bin/coverage.sh
```

## build

### builds sources

```sh
./bin/build.sh
```

## setup

### installs packages and requirements

```sh
./bin/setup.sh
```

## env

### sets up environment variables

```sh
./bin/env.sh
```

optional:

- [git](https://git-scm.com)
- [pyenv](https://github.com/pyenv/pyenv)
- [python](https://www.python.org)
- [cmake](https://cmake.org)
- [docker](https://docker.com)
- [cli](https://github.com/cli/cli/blob/trunk/docs/install_linux.md)
- [hooks](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks)

requirements:

- [rexo](https://github.com/christophercrouzet/rexo)
