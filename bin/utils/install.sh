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

libs=$(find "${pwd}/bin/libs" -type f -name "*.sh" -exec echo {} \;)
for i in $libs; do
    import="$(echo $i | sed -n -e 's/^.*bin\/libs\/\(.*\)$/\1/p')"
    . "${pwd}/bin/libs/${import}"
done

case "${install}" in

    "--env") # installs env variables ('. ./install.sh env')
        user=$(id -un)
        id=$(id -u)
        group=$(id -gn)
        USER_NAME=${USER_NAME:-$user}
        USER_GROUP=${USER_GROUP:-$group}
        USER_ID=${USER_ID:-$id}
        export USER_NAME=${USER_NAME}
        export USER_GROUP=${USER_GROUP}
        export USER_ID=${USER_ID}
        ;;

    "--asan") # installs asan env variables ('. ./install.sh asan')
        LSAN_OPTIONS=disable_coredump=1:handle_segv=0:verbosity=0:log_threads=0:log_pointers=1
        ASAN_OPTIONS=abort_on_error=1:report_objects=0:sleep_before_dying=0:verbosity=0:fast_unwind_on_malloc=0:detect_leaks=0
        export LSAN_OPTIONS=${LSAN_OPTIONS}
        export ASAN_OPTIONS=${ASAN_OPTIONS}
        ;;

    "--git") # installs git variables
        git config --global --add safe.directory "${pwd}"
        git config --global pull.rebase false
        ;;

    "--submodule") # installs rexo as git submodule
        git-submodule-install https://github.com/christophercrouzet/rexo.git src/rexo
        ;;

    "--submodule-lldb-mi") # installs lldb-mi as git submodule
        git-submodule-install https://github.com/lldb-tools/lldb-mi.git src/lldb-mi
        ;;

    "--hooks") # installs git hooks
        cp "${pwd}/.hooks/prepare-commit-msg" "${pwd}/.git/hooks/prepare-commit-msg"
        chmod u+x "${pwd}/.git/hooks/prepare-commit-msg"
        ;;

    "--pyenv") # downloads and installs pyenv
        rm -rf "${HOME}/.pyenv"
        curl --silent https://pyenv.run | bash
        ;;

    "--clang-format") # setup default .clang-format file
        clang-format -style="{BasedOnStyle: WebKit, IndentWidth: 4, BreakBeforeBraces: Attach}" --dump-config > .clang-format
        ;;

    "--clang-tidy") # setup default .clang-tidy file
        clang-tidy --format-style="{BasedOnStyle: WebKit, IndentWidth: 4, BreakBeforeBraces: Attach}" --dump-config > .clang-tidy
        ;;

    "--devcontainer-extensions") # installs vs code .devcontainers extensions
        code --install-extension ms-vscode-remote.remote-containers
        ;;

    "--docker-extensions") # installs vs code docker extensions
        code --install-extension ms-azuretools.vscode-docker
        ;;

    "--marp-extention") # istalls marp vscode extension for markdown presentation
        code --install-extension marp-team.marp-vscode
        npm install -g @marp-team/marp-cli
        ;;

    "--marp-cli") # istalls marp cli globally for markdown presentation
        npm install -g @marp-team/marp-cli
        ;;

    *)
        commands=$(cat $0 | sed -e 's/^[ \t]*//;' | sed -e '/^[ \t]*$/d' | sed -n -e 's/^"\(.*\)".*#/    \1:/p' | sed -n -e 's/: /:\n        /p')
        script="$(basename "$(test -L "$0" && readlink "$0" || echo "$0")")"
        help=$(\
cat << EOF
Installs optional dependencies
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
