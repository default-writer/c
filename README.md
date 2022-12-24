# c project description

- code coverage
- `List<T>` implementation

# shell

bootstraps build

```sh
./bin/runme.sh
```

## install

installs optional components:

```sh
./bin/install.sh
```

## uninstall

uninstalls optional components:

```sh
./bin/uninstall.sh
```

## cleanup

cleans up the root directory

- forsefully removes any generated folders
- breaks sonarlint and other directory based stuff (sonarlint searchs for **build** directory)

```sh
./bin/cleanup.sh
```

## coverage

builds sources with code coverage
 
```sh
./bin/coverage.sh
```

## build

builds sources

```sh
./bin/build.sh
```

## setup

installs packages and requirements

```sh
./bin/setup.sh
```

## env

setup environment variables

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
