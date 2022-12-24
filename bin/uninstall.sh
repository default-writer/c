#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ "${uid}" -eq 0 ]; then
	echo "Please run as user"
	exit
fi

pwd=$(pwd)

install="$1"

case "${install}" in

    "--git") # unsets git config global environment variables
        git config --global --unset safe.directory
        git config --global --unset pull.rebase
        ;;

    "--submodule") # deinits git submodule rexo and cleans up rexo directories
        git submodule deinit -f src/rexo
        rm -rf .git/modules/src/rexo
        git rm -f src/rexo
        ;;

    "--hooks") # removes prepare-commit-msg hook from .git
        rm -rf .git/hooks/prepare-commit-msg
        ;;

    "--pyenv") # removes .pyenv folder
    	rm -rf $HOME/.pyenv
        ;;

    *)
        commands=$(cat $0 | sed -e 's/^[ \t]*//;' | sed -e '/^[ \t]*$/d' | sed -n -e 's/^"\(.*\)".*#/    \1:/p' | sed -n -e 's/: /:\n        /p')
        script="$(basename "$(test -L "$0" && readlink "$0" || echo "$0")")"
        help=$(\
cat << EOF
Uninstalls optional dependencies
Usage: ${script} <option>
${commands}
EOF
)
        echo "${help}"
        exit
        ;;

esac

cd "${pwd}"
