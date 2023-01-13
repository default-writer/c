#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
    echo "Please run as user"
    exit
fi

pwd=$(pwd)

install="$1"

case "${install}" in

    "--zsh") # installs zsh using bash installation script
        curl --silent -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh | bash
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

cd "${pwd}"
