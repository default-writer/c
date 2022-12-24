#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ ! "${uid}" -eq 0 ]; then
    echo "Please run as root"
    exit
fi

pwd=$(pwd)

install="$1"

case "${install}" in

    "--llvm") # installs llvm and llvm-cov
        apt update -y
        apt install -y llvm
        apt upgrade -y
        ;;

    "--zsh") # installs zsh
        apt update -y
        apt install -y zsh
        apt upgrade -y
        chsh -s $(which bash)
        ;;

    "--gh") # installs gh
        apt update -y
        type -p curl >/dev/null || apt install curl -y
        curl --silent -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | dd of=/usr/shar\(e/keyrings/githubcli-archive-keyring.gpg \
        && chmod go+r /usr/share/keyrings/githubcli-archive-keyring.gpg \
        && echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | tee /etc/apt/sources.list.d/github-cli.list > /dev/null \
        && apt update -y \
        && apt install gh -y
        apt upgrade -y
        ;;

    "--git") # install git
        apt update -y
        apt install -y git
        apt upgrade -y
        ;;

    "--pyenv") # installs pyenv
        apt update -y
        apt install -y build-essential zlib1g-dev libffi-dev libssl-dev libbz2-dev libreadline-dev libsqlite3-dev liblzma-dev python3-tk tk-dev
        apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report
        apt upgrade -y
        ;;

    "--python") # installs python
        apt update -y
        apt install -y build-essential curl git ca-certificates python3 python3-dev python3-pip python3-venv python3-behave python3-virtualenv
        apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report
        apt upgrade -y
        ;;

    "--cmake") # installs cmake
        apt update -y
        apt install -y --no-install-recommends curl ca-certificates git build-essential lldb lcov cmake clangd g++ gcc gdb lcov ninja-build
        apt upgrade -y
        ;;

    "--docker") # installs docker
        apt update -y
        apt install -y \
            ca-certificates \
            curl \
            gnupg \
            lsb-release
        mkdir -p /etc/apt/keyrings
        curl --silent -fsSL --use-ascii --retry 5 --retry-all-errors https://download.docker.com/linux/ubuntu/gpg | gpg --dearmor -o /etc/apt/keyrings/docker.gpg
        echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | tee /etc/apt/sources.list.d/docker.list >/dev/null
        apt update -y
        chmod a+r /etc/apt/keyrings/docker.gpg
        apt update -y
        apt install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin
        apt upgrade -y
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

cd "${pwd}"
