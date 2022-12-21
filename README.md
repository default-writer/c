# c project description

- `List<T>` implementation
- **live** test/code coverage & updates ([vscode](https://code.visualstudio.com/) only)

# shell

To bootstrap build environment, run

```sh
./bin/runme.sh
```

## install

installs:

- [git](https://git-scm.com)
- [rexo](https://github.com/christophercrouzet/rexo)
- [hooks](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks)
- [pyenv](https://github.com/pyenv/pyenv#automatic-installer)

installs git

```sh
./bin/install.sh git
```

installs git submodule

```sh
./bin/install.sh submodule
```

installs git hooks

```sh
./bin/install.sh hooks
```

installs pyenv

```sh
./bin/pyenv.sh pyenv
```

## uninstall

uninstalls:

- [git](https://git-scm.com)
- [rexo](https://github.com/christophercrouzet/rexo)
- [hooks](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks)
- [pyenv](https://github.com/pyenv/pyenv#automatic-installer)

uninstalls git

```sh
./bin/uninstall.sh git
```

uninstalls git submodule

```sh
./bin/uninstall.sh submodule
```

uninstalls git hooks

```sh
./bin/uninstall.sh hooks
```

uninstalls pyenv

```sh
./bin/pyenv.sh pyenv
```

## cleanup

cleans up the root directory

forsefully removes any generated folders

breaks sonarlint and other directory based stuff (sonarlint searchs for **build** directory)

```sh
./bin/cleanup.sh
```

## coverage

runs code coverage init
 
```sh
./bin/coverage.sh
```

## build

runs buildig sources

restores sonarlint and other stuff (sonarlint searches for **build** directory)

```sh
./bin/build.sh
```

## setup

installs packages and requirements

- [git](https://git-scm.com)
- [pyenv](https://github.com/pyenv/pyenv)
- [python](https://www.python.org)
- [cmake](https://cmake.org)
- [docker](https://docker.com)
- [cli](https://github.com/cli/cli/blob/trunk/docs/install_linux.md)

setup git

```sh
./bin/setup.sh git
```

setup gh

```sh
./bin/setup.sh gh
```

setup pyenv

```sh
./bin/setup.sh pyenv
```

setup python

```sh
./bin/setup.sh python
```

setup cmake

```sh
./bin/setup.sh cmake
```

setup docker

```sh
./bin/setup.sh docker
```

## env