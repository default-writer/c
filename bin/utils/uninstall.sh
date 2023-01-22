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

main=$(find "${pwd}/bin/libs" -type f -name "*.sh" -exec echo {} \;)
for i in $main; do
    import="$(echo $i | sed -n -e 's/^.*bin\/libs\/\(.*\)$/\1/p')"
    . "${pwd}/bin/libs/${import}"
done

case "${install}" in

    "--git") # unsets git config global environment variables
        git config --global --unset safe.directory
        git config --global --unset pull.rebase
        ;;

    "--submodule-rexo") # deinits git submodule rexo and cleans up rexo directories
        git-submodule-uninstall src/rexo
        ;;

    "--submodule-lldb-mi") # installs lldb-mi as git submodule
        git-submodule-uninstall src/lldb-mi
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

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
