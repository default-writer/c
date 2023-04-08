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

. "${pwd}/bin/scripts/load.sh"

## Installs optional dependencies
## Usage: ${script} <option>
## ${commands}

case "${install}" in

    "--rustc") # installs rustc
        export DEBIAN_FRONTEND=noninteractive
        set -a && eval "$(sudo tee --append /etc/environment <<<'DEBIAN_FRONTEND=noninteractive')" && set +a
        curl --silent --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs -o /tmp/rustp-init.sh
        chmod +x /tmp/rustp-init.sh
        /tmp/rustp-init.sh -y --default-toolchain stable
        rm -f /tmp/rustp-init.sh
        . $HOME/.cargo/env
        cargo install slint-viewer
        ;;

    "--clangd") # installs clangd 15.0.6
        if [ ! -f "${pwd}/clangd/clangd_15.0.6/bin/clangd" ]; then
            [ ! -d "${pwd}/clangd" ] && mkdir "${pwd}/clangd"
            curl --silent -L https://github.com/clangd/clangd/releases/download/15.0.6/clangd-linux-15.0.6.zip -o "${pwd}/clangd/clangd-linux-15.0.6.zip"
            unzip -o -q "${pwd}/clangd/clangd-linux-15.0.6.zip" -d "${pwd}/clangd/"
            rm -f "${pwd}/clangd/clangd-linux-15.0.6.zip"
        fi
        ;;

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

    "--args") # install test .args file
        args=( '--all' '--clean' '--silent' '--sanitize' )
        printf '%s\n' "${args[@]}" > "${pwd}/.args"
        ;;

    "--submodule-rexo") # installs rexo as git submodule
        submodule-install https://github.com/christophercrouzet/rexo.git src/rexo
        ;;

    "--submodule-lldb-mi") # installs lldb-mi as git submodule
        submodule-install https://github.com/lldb-tools/lldb-mi.git src/lldb-mi
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
        code --install-extension ms-vscode-remote.remote-containers --force &>/dev/null
        ;;

    "--docker-extensions") # installs vs code docker extensions
        code --install-extension ms-azuretools.vscode-docker --force &>/dev/null
        ;;

    "--marp-extention") # istalls marp vscode extension
        code --install-extension marp-team.marp-vscode --force &>/dev/null
        ;;

    "--run-on-save") # istalls run on save vsode extension
        code --install-extension emeraldwalk.RunOnSave --force &>/dev/null
        ;;

    "--marp-cli") # istalls marp cli globally
        npm install -g @marp-team/marp-cli
        ;;

    *)
        help
        exit
        ;;

esac

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
