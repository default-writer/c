# c project description

- `List<T>` implementation
- **live** test/code coverage & updates ([vscode](https://code.visualstudio.com/) only)

# shell

## init

initializes repo:

- intializes [git](https://git-scm.com) **global** environment variables
- installs [rexo](https://github.com/christophercrouzet/rexo) as **git submodule**
- install git [hooks](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks)

initializes git

```sh
./bin/init.sh git
```

initializes git submodule

```sh
./bin/init.sh submodule
```

installs git hooks

```sh
./bin/init.sh hooks
```

## deinit

deinitializes repo:

- deintializes [git](https://git-scm.com) **global** environment variables
- deintializes [rexo](https://github.com/christophercrouzet/rexo) as **git submodule**

deinitializes git

```sh
./bin/deinit.sh git
```

deinitializes git submodule

```sh
./bin/deinit.sh submodule
```

deinstalls git hooks

```sh
./bin/deinit.sh hooks
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