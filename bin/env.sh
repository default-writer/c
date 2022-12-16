#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

# gets the current working directory
pwd=$(pwd)

install="$1"

if [ "${install}" == "pyenv" ]; then
	# adding pyenv section
	grep -qxF '# pyenv' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# pyenv' >> $HOME/.bashrc)
	grep -qxF 'export PYENV_ROOT="$HOME/.pyenv"' $HOME/.bashrc || echo 'export PYENV_ROOT="$HOME/.pyenv"' >> $HOME/.bashrc
	grep -qxF 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' $HOME/.bashrc || echo 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' >> $HOME/.bashrc
	grep -qxF 'eval "$(pyenv init -)"' $HOME/.bashrc || echo 'eval "$(pyenv init -)"' >> $HOME/.bashrc
	grep -qxF 'eval "$(pyenv virtualenv-init -)"' $HOME/.bashrc || echo 'eval "$(pyenv virtualenv-init -)"' >> $HOME/.bashrc	
fi

if [ "${install}" == "user" ]; then
	# adding user section
	grep -qxF '# user' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# user' >> $HOME/.bashrc)
	grep -qxF 'export USER_NAME=$USER' $HOME/.bashrc || echo 'export USER_NAME=$USER' >> $HOME/.bashrc
	grep -qxF 'export USER_ID=$(id -u)' $HOME/.bashrc || echo 'export USER_ID=$(id -u)' >> $HOME/.bashrc
	grep -qxF 'export USER_GROUP=$(id -gn)' $HOME/.bashrc || echo 'export USER_GROUP=$(id -gn)' >> $HOME/.bashrc
fi

if [ "${install}" == "nvm" ]; then
	# adding nvm section
	grep -qxF '# nvm' $HOME/.bashrc || (tail -1 $HOME/.bashrc | grep -qxF '' || echo '' >> $HOME/.bashrc && echo '# nvm' >> $HOME/.bashrc)
	grep -qxF 'export NVM_DIR="$HOME/.nvm"' $HOME/.bashrc || echo 'export NVM_DIR="$HOME/.nvm"' >> $HOME/.bashrc
	grep -qxF '[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"' $HOME/.bashrc || echo '[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"' >> $HOME/.bashrc
	grep -qxF '[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"' $HOME/.bashrc || echo '[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"' >> $HOME/.bashrc
fi

. $HOME/.bashrc

# returns to current working directory
cd "${pwd}"
