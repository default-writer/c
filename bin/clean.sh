#!/usr/bin/env bash
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

opts=( "${@:2}" )

function help() {
        commands=$(cat $0 | sed -e 's/^[ \t]*//;' | sed -e '/^[ \t]*$/d' | sed -n -e 's/^"\(.*\)".*#/    \1:/p' | sed -n -e 's/: /:\n        /p')
        script="$(basename "$(test -L "$0" && readlink "$0" || echo "$0")")"
        help=$(\
cat << EOF
Builds binaries
Usage: ${script} <option>
${commands}
EOF
)
        echo "${help}"
        exit
}

case "${install}" in

    "--all") # cleans up uncommited changes and non-gited files and folders
        ;;

    *)
        help    
        ;;

esac

git clean -f -q -d -x

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"