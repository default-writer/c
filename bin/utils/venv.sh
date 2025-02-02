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
    "--venv") # installs python3 venv virtual environment into .venv folder
        rm -rf .venv
        python3 -m venv .venv
        ;;

    "--pyenv") # installs pyenv virtual environment for 3.11.1 into .venv folder
        pyenv install -f 3.11.1
        pyenv virtualenv -f 3.11.1 .venv
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
