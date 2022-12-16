# c project description

- `List<T>` implementation
- **live** test/code coverage & updates ([vscode](https://code.visualstudio.com/) only)

# shell

## initialize

initializes repo:

- [git](https://git-scm.com)
- [rexo](https://github.com/christophercrouzet/rexo)
- [hooks](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks)
- [pyenv](https://github.com/pyenv/pyenv#automatic-installer)

installs git

```sh
./bin/init.sh git
```

installs git submodule

```sh
./bin/init.sh submodule
```

installs git hooks

```sh
./bin/init.sh hooks
```

installs pyenv

```sh
./bin/pyenv.sh pyenv
```

## uninstall

uninstalls repo:

- [git](https://git-scm.com)
- [rexo](https://github.com/christophercrouzet/rexo)

uninstalls git

```sh
./bin/deinit.sh git
```

uninstalls git submodule

```sh
./bin/deinit.sh submodule
```

uninstalls git hooks

```sh
./bin/deinit.sh hooks
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

setup git

```sh
./bin/setup.sh git
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