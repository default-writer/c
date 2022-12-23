#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

install="$1"
option="$2"

if [ "${install}" == "zsh" ]; then
	curl --silent -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh | bash
fi

if [ "${install}" == "asan" ]; then
	grep -qxF '# asan' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# asan' >> $HOME/.bashrc)
	grep -qxF 'export LSAN_OPTIONS=disable_coredump=1:handle_segv=0:verbosity=0:log_threads=0:log_pointers=1' $HOME/.bashrc || echo 'export LSAN_OPTIONS=disable_coredump=1:handle_segv=0:verbosity=0:log_threads=0:log_pointers=1' >> $HOME/.bashrc
	grep -qxF 'export ASAN_OPTIONS=abort_on_error=1:report_objects=0:sleep_before_dying=0:verbosity=0:fast_unwind_on_malloc=0:detect_leaks=0' $HOME/.bashrc || echo 'export ASAN_OPTIONS=abort_on_error=1:report_objects=0:sleep_before_dying=0:verbosity=0:fast_unwind_on_malloc=0:detect_leaks=0' >> $HOME/.bashrc
fi

if [ "${install}" == "pyenv" ]; then
	grep -qxF '# pyenv' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# pyenv' >> $HOME/.bashrc)
	grep -qxF 'export PYENV_VIRTUALENV_DISABLE_PROMPT=1' $HOME/.bashrc || echo 'export PYENV_VIRTUALENV_DISABLE_PROMPT=1' >> $HOME/.bashrc
	grep -qxF 'export PYENV_ROOT="$HOME/.pyenv"' $HOME/.bashrc || echo 'export PYENV_ROOT="$HOME/.pyenv"' >> $HOME/.bashrc
	grep -qxF 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' $HOME/.bashrc || echo 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' >> $HOME/.bashrc
	grep -qxF 'eval "$(pyenv init -)"' $HOME/.bashrc || echo 'eval "$(pyenv init -)"' >> $HOME/.bashrc
	grep -qxF 'eval "$(pyenv virtualenv-init -)"' $HOME/.bashrc || echo 'eval "$(pyenv virtualenv-init -)"' >> $HOME/.bashrc	
fi

if [ "${install}" == "user" ]; then
	grep -qxF '# user' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# user' >> $HOME/.bashrc)
	grep -qxF 'export USER_NAME=$(id -un)' $HOME/.bashrc || echo 'export USER_NAME=$(id -un)' >> $HOME/.bashrc
	grep -qxF 'export USER_GROUP=$(id -gn)' $HOME/.bashrc || echo 'export USER_GROUP=$(id -gn)' >> $HOME/.bashrc
	grep -qxF 'export USER_ID=$(id -u)' $HOME/.bashrc || echo 'export USER_ID=$(id -u)' >> $HOME/.bashrc
fi

if [ "${install}" == "nvm" ]; then
	grep -qxF '# nvm' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# nvm' >> $HOME/.bashrc)
	grep -qxF 'export NVM_DIR="$HOME/.nvm"' $HOME/.bashrc || echo 'export NVM_DIR="$HOME/.nvm"' >> $HOME/.bashrc
	grep -qxF '[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"' $HOME/.bashrc || echo '[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"' >> $HOME/.bashrc
	grep -qxF '[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"' $HOME/.bashrc || echo '[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"' >> $HOME/.bashrc
fi

. $HOME/.bashrc

cd "${pwd}"
