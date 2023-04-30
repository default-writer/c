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

opts=( "${@:2}" )

. "${pwd}/bin/scripts/load.sh"

## Installs build system dependencies
## Usage: ${script} <option> [optional]
## ${commands}

case "${install}" in

    "")
        ;;

    *)
        help
        ;;

esac

for opt in ${opts[@]}; do
    case ${opt} in

        "")
            ;;

        "--no-update") # [optional] skips system updates
            update-flags="--no-update"
            ;;

        "--no-upgrade") # [optional] skips system upgrades
            updgrade-flags="--no-upgrade"
            ;;

        "--help") # shows command desctiption
            help
            ;;

        *)
            ;;

    esac
done

case "${install}" in

    "--nodejs") # installs nodejs
        update ${update-flags}
        apt install -y nodejs
        upgrade ${updgrade-flags}
        ;;

    "--gtk4") # installs GTK 4
        update ${update-flags}
        apt install -y gnome-devel libgtk-4-1 libgtk2.0-dev libgtk-3-dev libgtk-4-dev libglib2.0-dev
        upgrade ${updgrade-flags}
        ;;

    "--qemu") # installs QEMU/KVM
        update ${update-flags}
        apt install -y qemu-kvm libvirt-clients libvirt-daemon-system bridge-utils virtinst libvirt-daemon
        DEBIAN_FRONTEND=noninteractive apt install -y --only-upgrade qemu-block-extra qemu-system-common qemu-system-data qemu-system-gui qemu-system-x86 qemu-utils tzdata
        upgrade ${updgrade-flags}
        ;;

    "--virt-manger") # installs virt-manager
        update ${update-flags}
        apt install -y virt-manager
        upgrade ${updgrade-flags}
        ;;

    "--enable-libvirtd") # enables libvirtd
        update ${update-flags}
        systemctl enable --now libvirtd
        upgrade ${updgrade-flags}
        ;;

    "--rustc") # installs rustc
        update ${update-flags}
        apt install -y --only-upgrade gdm3 gir1.2-gdm-1.0 libgdm1 qemu-block-extra qemu-system-common qemu-system-data qemu-system-gui qemu-system-x86 qemu-utils
        apt install -y cargo pkg-config fontconfig
        apt install -y libfontconfig1-dev libfontconfig
        upgrade ${updgrade-flags}
        ;;

    "--configuration") # installs keyboard-configuration
        update ${update-flags}
        DEBIAN_FRONTEND=noninteractive apt-get install -y keyboard-configuration gettext-base
        apt install -y --no-install-recommends curl ca-certificates git build-essential lldb lcov cmake clangd g++ gcc gdb lcov ninja-build pkg-config
        apt install -y --only-upgrade distro-info-data
        upgrade ${updgrade-flags}
        ;;


    "--llvm") # installs llvm and llvm-cov
        update ${update-flags}
        apt install -y llvm
        upgrade ${updgrade-flags}
        ;;

    "--zsh") # installs zsh
        update ${update-flags}
        apt install -y zsh
        chsh -s $(which bash)
        upgrade ${updgrade-flags}
        ;;

    "--bazel") # installs bazel
        update ${update-flags}
        apt install -y apt-transport-https curl gnupg
        curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --batch --yes --dearmor >bazel-archive-keyring.gpg
        mv bazel-archive-keyring.gpg /usr/share/keyrings
        echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list
        apt install -y bazel
        upgrade ${updgrade-flags}
        ;;

    "--gh") # installs gh
        update ${update-flags}
        type -p curl >/dev/null || apt install curl -y
        curl --silent -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | dd of=/usr/shar\(e/keyrings/githubcli-archive-keyring.gpg
        chmod go+r /usr/share/keyrings/githubcli-archive-keyring.gpg
        echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | tee /etc/apt/sources.list.d/github-cli.list >/dev/null
        apt install -y gh
        upgrade ${updgrade-flags}
        ;;

    "--git") # installs git
        update ${update-flags}
        apt install -y git
        upgrade ${updgrade-flags}
        ;;

    "--colordiff") # installs colordiff
        update ${update-flags}
        apt install -y colordiff
        upgrade ${updgrade-flags}
        ;;

    "--meld") # installs meld
        update ${update-flags}
        apt install -y meld
        upgrade ${updgrade-flags}
        ;;

    "--sublime-merge") # installs sublime-merge
        update ${update-flags}
        apt install -y apt-transport-https
        wget -qO - https://download.sublimetext.com/sublimehq-pub.gpg | gpg --batch --yes --dearmor | tee /etc/apt/trusted.gpg.d/sublimehq-archive.gpg >/dev/null
        echo "deb https://download.sublimetext.com/ apt/stable/" | tee /etc/apt/sources.list.d/sublime-text.list
        apt update -y
        apt install -y sublime-merge
        upgrade ${updgrade-flags}
        ;;

    "--pyenv") # installs pyenv
        update ${update-flags}
        apt install -y build-essential zlib1g-dev libffi-dev libssl-dev libbz2-dev libreadline-dev libsqlite3-dev liblzma-dev python3-tk tk-dev
        apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire libpipewire-0.3-0 libpipewire-0.3-common libpipewire-0.3-modules open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
        upgrade ${updgrade-flags}
        ;;

    "--python") # installs python
        update ${update-flags}
        apt install -y build-essential curl git ca-certificates python3 python3-dev python3-pip python3-venv python3-behave python3-virtualenv
        apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire libpipewire-0.3-0 libpipewire-0.3-common libpipewire-0.3-modules open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
        upgrade ${updgrade-flags}
        ;;

    "--clang-lldb-mi") # installs lldb-mi dependencies
        update ${update-flags}
        apt-get install -y libclang-dev liblldb-dev || apt-get install libclang-6.0-dev liblldb-6.0-dev || apt-get install libclang-4.0-dev liblldb-4.0-dev || apt-get install libclang-3.8-dev liblldb-3.8-dev
        apt install python3-lldb-14
        ln -s /usr/lib/llvm-14/lib/python3.10/dist-packages/lldb/* /usr/lib/python3/dist-packages/lldb/
        upgrade ${updgrade-flags}
        ;;

    "--clang-format") # installs clang-format
        update ${update-flags}
        # wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
        # build=$(echo $(lsb_release -a 2>&1 | tail -1 | sed  -e 's/\w*:\s*//g'))
        # repository=$(echo deb http://apt.llvm.org/${build} llvm-toolchain-${build} main)
        # add-apt-repository --yes "${repository}"
        # upgrade
        apt install -y clang-format
        apt install -y apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire  open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
        apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire  open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
        upgrade ${updgrade-flags}
        ;;

    "--cmake") # installs cmake
        update ${update-flags}
        apt install -y --no-install-recommends curl ca-certificates git build-essential lldb lcov cmake clangd clang-format g++ gcc gdb lcov ninja-build bison
        upgrade ${updgrade-flags}
        ;;

    "--docker") # installs docker
        update ${update-flags}
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
        upgrade ${updgrade-flags}
        ;;

    "--docker-compose") # installs docker-compose
        update ${update-flags}
        apt install -y \
            desktop-file-utils
        apt install -y --fix-broken qemu-kvm libvirt-clients libvirt-daemon-system bridge-utils virtinst libvirt-daemon pass uidmap
        systemctl enable --now libvirtd
        curl --silent -L https://desktop.docker.com/linux/main/amd64/docker-desktop-4.15.0-amd64.deb -o /tmp/docker-desktop-4.15.0-amd64.deb
        apt update -y
        dpkg -i /tmp/docker-desktop-4.15.0-amd64.deb
        apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire libpipewire-0.3-0 libpipewire-0.3-common libpipewire-0.3-modules open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
        upgrade ${updgrade-flags}
        ;;

    "--gh-cli") # installs github cli
        update ${update-flags}
        [ ! -d "${pwd}/gh" ] && mkdir "${pwd}/gh" ]
        curl -L "https://github.com/cli/cli/releases/download/v2.22.0/gh_2.22.0_linux_amd64.deb" -o "${pwd}/gh/gh_2.22.0_linux_amd64.deb"
        dpkg -i "${pwd}/gh/gh_2.22.0_linux_amd64.deb"
        ;;

    *)
        help
        exit
        ;;

esac

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
