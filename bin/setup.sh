#!/usr/bin/env bash
set -e

err_report() {
    echo "ERROR: on line $*: $(cat $0 | sed $1!d)" >&2
}

trap 'err_report $LINENO' ERR

uid=$(id -u)

if [ ! "${uid}" -eq 0 ]; then
    echo "Please run as root"
    exit
fi

pwd=$(pwd)

install="$1"

function update() {
    apt install -y --fix-broken
    apt update -y --fix-missing
}

function upgrade() {
    apt upgrade -y
    apt autoremove -y
    apt full-upgrade -y
}

case "${install}" in

    "--llvm") # installs llvm and llvm-cov
        update
        apt install -y llvm
        upgrade
        ;;

    "--zsh") # installs zsh
        update
        apt install -y zsh
        upgrade
        chsh -s $(which bash)
        upgrade
        ;;

    "--bazel") # installs bazel
        update
        apt install -y apt-transport-https curl gnupg
        curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --batch --yes --dearmor >bazel-archive-keyring.gpg
        sudo mv bazel-archive-keyring.gpg /usr/share/keyrings
        echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list
        update
        apt install -y bazel
        upgrade
        ;;

    "--gh") # installs gh
        update
        type -p curl >/dev/null || apt install curl -y
        curl --silent -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | dd of=/usr/shar\(e/keyrings/githubcli-archive-keyring.gpg
        chmod go+r /usr/share/keyrings/githubcli-archive-keyring.gpg
        echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | tee /etc/apt/sources.list.d/github-cli.list >/dev/null
        update
        apt install -y gh
        upgrade
        ;;

    "--git") # installs git
        update
        apt install -y git
        upgrade
        ;;

    "--colordiff") # installs colordiff
        update
        apt install -y git
        upgrade
        ;;

    "--meld") # installs meld
        update
        apt install -y meld
        upgrade
        ;;

    "--sublime-merge") # installs sublime-merge
        update
        apt install -y apt-transport-https
        wget -qO - https://download.sublimetext.com/sublimehq-pub.gpg | gpg --batch --yes --dearmor | tee /etc/apt/trusted.gpg.d/sublimehq-archive.gpg >/dev/null
        echo "deb https://download.sublimetext.com/ apt/stable/" | tee /etc/apt/sources.list.d/sublime-text.list
        apt update -y
        apt install -y sublime-merge
        upgrade
        ;;

    "--pyenv") # installs pyenv
        update
        apt install -y build-essential zlib1g-dev libffi-dev libssl-dev libbz2-dev libreadline-dev libsqlite3-dev liblzma-dev python3-tk tk-dev
        apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire libpipewire-0.3-0 libpipewire-0.3-common libpipewire-0.3-modules open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
        upgrade
        ;;

    "--python") # installs python
        update
        apt install -y build-essential curl git ca-certificates python3 python3-dev python3-pip python3-venv python3-behave python3-virtualenv
        apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire libpipewire-0.3-0 libpipewire-0.3-common libpipewire-0.3-modules open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
        upgrade
        ;;

    "--clang-format") # installs clang-format
        update
        apt install -y clang-format
        apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire libpipewire-0.3-0 libpipewire-0.3-common libpipewire-0.3-modules open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
        upgrade
        ;;

    "--cmake") # installs cmake
        update
        apt install -y --no-install-recommends curl ca-certificates git build-essential lldb lcov cmake clangd clang-format g++ gcc gdb lcov ninja-build
        upgrade
        ;;

    "--docker") # installs docker
        update
        apt install -y \
            desktop-file-utils \
            ca-certificates \
            curl \
            gnupg \
            lsb-release
        mkdir -p /etc/apt/keyrings
        curl --silent -fsSL --use-ascii --retry 5 --retry-all-errors https://download.docker.com/linux/ubuntu/gpg | gpg --batch --yes --dearmor -o /etc/apt/keyrings/docker.gpg
        echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | tee /etc/apt/sources.list.d/docker.list >/dev/null
        apt update -y
        chmod a+r /etc/apt/keyrings/docker.gpg
        apt install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin
        usermod -aG docker $USER
        chmod 666 /var/run/docker.sock
        upgrade
        ;;

    "--docker-compose") # installs docker-compose
        update
        apt install -y \
            desktop-file-utils
        apt install -y --fix-broken qemu-kvm libvirt-clients libvirt-daemon-system bridge-utils virtinst libvirt-daemon pass uidmap
        systemctl enable --now libvirtd
        curl -L https://desktop.docker.com/linux/main/amd64/docker-desktop-4.15.0-amd64.deb -o /tmp/docker-desktop-4.15.0-amd64.deb
        apt update -y
        dpkg -i /tmp/docker-desktop-4.15.0-amd64.deb
        apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire libpipewire-0.3-0 libpipewire-0.3-common libpipewire-0.3-modules open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
        upgrade
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
