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

    "--appwrite") # installs appwrite. appwrite is a self-hosted backend-as-a-service platform that provides developers with all the core APIs required to build any application.
        docker run -it --rm \
            --volume /var/run/docker.sock:/var/run/docker.sock \
            --volume "$(pwd)"/appwrite:/usr/src/code/appwrite:rw \
            --entrypoint="install" \
            appwrite/appwrite:1.3.1
        ;;

    "--appwrite-start") # starts appwrite docker
        docker compose -f "${pwd}/appwrite/docker-compose.yml" up -d --remove-orphans --renew-anon-volumes
        ;;

    "--rustc") # installs rustc
        DEBIAN_FRONTEND=noninteractive apt-get install -y keyboard-configuration gettext-base
        curl --silent --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs -o /tmp/rustp-init.sh
        chmod +x /tmp/rustp-init.sh
        /tmp/rustp-init.sh -y --default-toolchain stable
        rm -f /tmp/rustp-init.sh
        . $HOME/.cargo/env
        cargo install slint-viewer
        ;;

    "--clangd") # installs clangd 16.0.2
        [ ! -d "${pwd}/.tools" ] && mkdir "${pwd}/.tools"
        [ ! -d "${pwd}/.tools/clangd_16.0.2" ] && mkdir "${pwd}/.tools/clangd_16.0.2"
        wget https://github.com/clangd/clangd/releases/download/16.0.2/clangd-linux-16.0.2.zip -qO "/tmp/clangd-linux-16.0.2.zip"
        unzip -o -q "/tmp/clangd-linux-16.0.2.zip" -d "/tmp"
        cp -r "/tmp/clangd_16.0.2/." "${pwd}/.tools/clangd_16.0.2"
        rm -rf "/tmp/clangd-linux-16.0.2"
        rm -f "/tmp/clangd-linux-16.0.2.zip"
        ;;

    "--cmake") # installs cmake
        [ ! -d "${pwd}/.tools" ] && mkdir "${pwd}/.tools"
        [ ! -d "${pwd}/.tools/cmake-3.25" ] && mkdir "${pwd}/.tools/cmake-3.25"
        wget https://github.com/Kitware/CMake/releases/download/v3.25.3/cmake-3.25.3-linux-x86_64.sh -qO "/tmp/cmake-3.25.3-linux-x86_64.sh"
        chmod +x "/tmp/cmake-3.25.3-linux-x86_64.sh"
        DEBIAN_FRONTEND=noninteractive /tmp/cmake-3.25.3-linux-x86_64.sh --prefix=${pwd}/.tools/cmake-3.25 --skip-license
        rm "/tmp/cmake-3.25.3-linux-x86_64.sh"
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
        submodule-install https://github.com/christophercrouzet/rexo.git .deps/rexo
        ;;

    "--submodule-vcpkg") # installs rexo as git submodule
        submodule-install https://github.com/Microsoft/vcpkg.git .deps/vcpkg
        ;;

    "--submodule-lldb-mi") # installs lldb-mi as git submodule
        submodule-install https://github.com/lldb-tools/lldb-mi.git .deps/lldb-mi
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
