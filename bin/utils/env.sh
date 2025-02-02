#!/usr/bin/env bash
if [[ "${BASHOPTS}" != *extdebug* ]]; then
    set -e
fi

err_report() {
    cd ${source}
    echo "ERROR: $0:$*"
    exit 8
}

if [[ "${BASHOPTS}" != *extdebug* ]]; then
    trap 'err_report $LINENO' ERR
fi

source=$(pwd)

pwd=$(cd "$(dirname $(dirname $(dirname "${BASH_SOURCE[0]}")))" &> /dev/null && pwd)

install="$1"

case "${install}" in

    "--zsh") # installs zsh using bash installation script
        curl --silent -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh | bash
        ;;

    "--mojo-sdk") # installs mojo sdk environment variables in .bashrc
        grep -qxF '# mojo' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# mojo' >> $HOME/.bashrc)
        grep -qxF 'export MODULAR_HOME=$HOME/.modular' $HOME/.bashrc || echo 'export MODULAR_HOME=$HOME/.modular' >> $HOME/.bashrc
        grep -qxF 'export PATH="$HOME/.modular/pkg/packages.modular.com_mojo/bin:$PATH"' $HOME/.bashrc || echo 'export PATH="$HOME/.modular/pkg/packages.modular.com_mojo/bin:$PATH"' >> $HOME/.bashrc
        grep -qxF 'export MOJO_PYTHON_LIBRARY="/usr/lib/x86_64-linux-gnu/libpython3.10.so"' $HOME/.bashrc || echo 'export MOJO_PYTHON_LIBRARY="/usr/lib/x86_64-linux-gnu/libpython3.10.so"' >> $HOME/.bashrc
        ;;

    "--asan") # installs asan environment variables in .bashrc
        grep -qxF '# asan' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# asan' >> $HOME/.bashrc)
        grep -qxF 'export LSAN_OPTIONS=disable_coredump=1:handle_segv=0:verbosity=0:log_threads=0:log_pointers=1' $HOME/.bashrc || echo 'export LSAN_OPTIONS=disable_coredump=1:handle_segv=0:verbosity=0:log_threads=0:log_pointers=1' >> $HOME/.bashrc
        grep -qxF 'export ASAN_OPTIONS=abort_on_error=1:report_objects=0:sleep_before_dying=0:verbosity=0:fast_unwind_on_malloc=0:detect_leaks=0' $HOME/.bashrc || echo 'export ASAN_OPTIONS=abort_on_error=1:report_objects=0:sleep_before_dying=0:verbosity=0:fast_unwind_on_malloc=0:detect_leaks=0' >> $HOME/.bashrc
        ;;

    "--pyenv") # installs pyenv environment variables in .bashrc
        grep -qxF '# pyenv' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# pyenv' >> $HOME/.bashrc)
        grep -qxF 'export PYENV_VIRTUALENV_DISABLE_PROMPT=1' $HOME/.bashrc || echo 'export PYENV_VIRTUALENV_DISABLE_PROMPT=1' >> $HOME/.bashrc
        grep -qxF 'export PYENV_ROOT="$HOME/.pyenv"' $HOME/.bashrc || echo 'export PYENV_ROOT="$HOME/.pyenv"' >> $HOME/.bashrc
        grep -qxF 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' $HOME/.bashrc || echo 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' >> $HOME/.bashrc
        grep -qxF 'eval "$(pyenv init -)"' $HOME/.bashrc || echo 'eval "$(pyenv init -)"' >> $HOME/.bashrc
        grep -qxF 'eval "$(pyenv virtualenv-init -)"' $HOME/.bashrc || echo 'eval "$(pyenv virtualenv-init -)"' >> $HOME/.bashrc
        ;;

    "--user") # installs user environment variables in .bashrc
        grep -qxF '# user' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# user' >> $HOME/.bashrc)
        grep -qxF 'export USER_NAME=$(id -un)' $HOME/.bashrc || echo 'export USER_NAME=$(id -un)' >> $HOME/.bashrc
        grep -qxF 'export USER_GROUP=$(id -gn)' $HOME/.bashrc || echo 'export USER_GROUP=$(id -gn)' >> $HOME/.bashrc
        grep -qxF 'export USER_ID=$(id -u)' $HOME/.bashrc || echo 'export USER_ID=$(id -u)' >> $HOME/.bashrc
        ;;

    "--nvm") # installs nvm environment variables in .bashrc
        grep -qxF '# nvm' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# nvm' >> $HOME/.bashrc)
        grep -qxF 'export NVM_DIR="$HOME/.nvm"' $HOME/.bashrc || echo 'export NVM_DIR="$HOME/.nvm"' >> $HOME/.bashrc
        grep -qxF '[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"' $HOME/.bashrc || echo '[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"' >> $HOME/.bashrc
        grep -qxF '[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"' $HOME/.bashrc || echo '[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"' >> $HOME/.bashrc
        ;;

    "--bun") # install bun environment variables in .bashrc
        grep -qxF '# bun' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# bun' >> $HOME/.bashrc)
        grep -qxF 'export BUN_INSTALL="$HOME/.bun"' $HOME/.bashrc || echo 'export BUN_INSTALL="$HOME/.bun"' >> $HOME/.bashrc
        grep -qxF 'export PATH=$BUN_INSTALL/bin:$PATH' $HOME/.bashrc || echo 'export PATH=$BUN_INSTALL/bin:$PATH' >> $HOME/.bashrc
        ;;

    "--bazel") # installs bazel environment variables in .bashrc
        grep -qxF '# bazel' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# bazel' >> $HOME/.bashrc)
        grep -qxF 'export BAZEL_ROOT="$HOME/.bazel"' $HOME/.bashrc || echo 'export BAZEL_ROOT="$HOME/.bazel"' >> $HOME/.bashrc
        grep -qxF 'export PATH="$BAZEL_ROOT:$BAZEL_ROOT/tools:$PATH"' $HOME/.bashrc || echo 'export PATH="$BAZEL_ROOT:$BAZEL_ROOT/tools:$PATH"' >> $HOME/.bashrc
        ;;

    "--gtk") # installs GTK 4 environment variables in .bashrc
        GTK_INCLUDE_DIRS=$(echo $(pkg-config --cflags gtk4))
        GTK_LIBRARY_DIRS=$(echo $(pkg-config --libs gtk4))
        grep -qxF '# gtk' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# gtk' >> $HOME/.bashrc)
        grep -qxF 'export GTK_INCLUDE_DIRS="' $HOME/.bashrc || echo 'export GTK_INCLUDE_DIRS="' ${GTK_INCLUDE_DIRS} '"' >> $HOME/.bashrc
        grep -qxF 'export GTK_LIBRARY_DIRS="' $HOME/.bashrc || echo 'export GTK_LIBRARY_DIRS="' ${GTK_LIBRARY_DIRS} '"' >> $HOME/.bashrc
        ;;

    "--dotnet") # installs dotnet environment variables in .bashrc
        grep -qxF '# dotnet 7' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# dotnet 7' >> $HOME/.bashrc)
        grep -qxF 'export DOTNET_ROOT="$HOME/.dotnet"' $HOME/.bashrc || echo 'export DOTNET_ROOT="$HOME/.dotnet"' >> $HOME/.bashrc
        grep -qxF 'export PATH="$DOTNET_ROOT:$DOTNET_ROOT/tools:$PATH"' $HOME/.bashrc || echo 'export PATH="$DOTNET_ROOT:$DOTNET_ROOT/tools:$PATH"' >> $HOME/.bashrc
        ;;

    *)
        commands=$(cat $0 | sed -e 's/^[ \t]*//;' | sed -e '/^[ \t]*$/d' | sed -n -e 's/^"\(.*\)".*#/    \1:/p' | sed -n -e 's/: /:\n        /p')
        script="$(basename "$(test -L "$0" && readlink "$0" || echo "$0")")"
        help=$(\
cat << EOF
Builds main test executables into build folder
Usage: ${script} <option> [--clean]
${commands}
EOF
)
        echo "${help}"
        exit
        ;;

esac

. $HOME/.bashrc

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
