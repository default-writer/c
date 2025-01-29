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

uid=$(id -u)

source=$(pwd)

pwd=$(cd "$(dirname $(dirname $(dirname "${BASH_SOURCE[0]}")))" &> /dev/null && pwd)

install="$1"

. "${pwd}/bin/scripts/load.sh"

if [[ "${install}" == "--help" ]]; then
    help
    exit
fi

if [ ! "${uid}" -eq 0 ]; then
    echo "Please run as root"
    exit
fi

updateflags="--update"
updgradeflags="--upgrade"

## Installs build system dependencies
## Usage: ${script} <option> [optional]
## ${commands}

while (($#)); do
    case "$1" in

        "--no-update") # [optional] skips system updates
            updateflags="--no-update"
            ;;

        "--no-upgrade") # [optional] skips system upgrades
            updgradeflags="--no-upgrade"
            ;;

        "--update") # runs system update
            update ${updateflags}
            ;;

        "--upgrade") # runs system upgrade
            upgrade ${updgradeflags}
            ;;

        "--mojo-sdk") # installs mojo sdk
            update ${updateflags}
            apt-get install modular
            modular clean
            modular install mojo
            upgrade ${updgradeflags}
            ;;

        "--nasm") # installs nasm
            update ${updateflags}
            apt-get install nasm
            upgrade ${updgradeflags}
            ;;

        "--mpv") # installs mpv
            update ${updateflags}
            apt install -y mpv
            upgrade ${updgradeflags}
            ;;

        "--raylib") # installs raylib
            update ${updateflags}
            apt install -y libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
            upgrade ${updgradeflags}
            ;;

        "--far2l") # installs far2l from ppa
            update ${updateflags}
            add-apt-repository ppa:far2l-team/ppa -y
            apt-get install far2l -y
            upgrade ${updgradeflags}
            ;;

        "--wsl2") # fixes wsl error /sbin/ldconfig.real: /usr/lib/wsl/lib/libcuda.so.1 is not a symbolic link
            mkdir /usr/lib/wsl/lib2
            ln -s /usr/lib/wsl/lib/* /usr/lib/wsl/lib2
            echo /usr/lib/wsl/lib2 | tee /etc/ld.so.conf.d/ld.wsl.conf
            ;;

        "--clangd") # installs llvm 18 system wide
            curl -L "https://apt.llvm.org/llvm.sh" -o "/tmp/llvm.sh"
            chmod u+x /tmp/llvm.sh
            /tmp/llvm.sh 18
            rm -r /tmp/llvm.sh
            ;;


        "--doxygen") # installs doxygen
            update ${updateflags}
            apt install -y --only-upgrade libsmbclient libwbclient0 samba-libs
            apt install -y flex bison libc6 doxygen graphviz texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra
            upgrade ${updgradeflags}
            ;;

        "--zip") # installs unzip
            update ${updateflags}
            apt install -y unzip
            upgrade ${updgradeflags}
            ;;

        "--nodejs") # installs nodejs
            update ${updateflags}
            apt install -y nodejs
            upgrade ${updgradeflags}
            ;;

        "--dotnet") # installs dotnet
            update ${updateflags}
            curl -sL https://dot.net/v1/dotnet-install.sh -o /tmp/dotnet-install.sh
            chmod +x /tmp/dotnet-install.sh
            bash /tmp/dotnet-install.sh --version 6.0.408 --channel 6.0
            bash /tmp/dotnet-install.sh --version 7.0.203 --channel 7.0
            rm -f /tmp/dotnet-install.sh
            upgrade ${updgradeflags}
            ;;

        "--powershell") # installs powershell
            update ${updateflags}
            apt install -y wget curl apt-transport-https software-properties-common
            curl -sL "https://packages.microsoft.com/config/ubuntu/$(lsb_release -rs)/packages-microsoft-prod.deb" -o /tmp/packages-microsoft-prod.deb
            dpkg -i /tmp/packages-microsoft-prod.deb
            apt update -y
            apt install -y powershell
            rm -f /tmp/packages-microsoft-prod.deb
            upgrade ${updgradeflags}
            ;;
            
        "--curl-openssl") # installs curl development library
            apt-get install libcurl4-openssl-dev
            ;;

        "--nodejs-bin") # installs nodejs using binary distribution
            update ${updateflags}
            apt install -y --only-upgrade libsmbclient libwbclient0 qemu-block-extra qemu-system-common qemu-system-data qemu-system-gui qemu-system-x86 qemu-utils samba-libs
            apt-get install -y ca-certificates curl gnupg
            mkdir -p /etc/apt/keyrings        
            curl -fsSL https://deb.nodesource.com/gpgkey/nodesource-repo.gpg.key | sudo gpg --dearmor -o /etc/apt/keyrings/nodesource.gpg
            apt update -y
            chmod a+r /etc/apt/keyrings/nodesource.gpg
            apt install -y nodejs
            upgrade ${updgradeflags}
            ;;

        "--npm") # installs npm
            update ${updateflags}
            apt install -y npm
            upgrade ${updgradeflags}
            ;;

        "--gtk4") # installs GTK 4
            update ${updateflags}
            apt install -y gnome-devel libgtk-4-1 libgtk2.0-dev libgtk-3-dev libgtk-4-dev libglib2.0-dev
            upgrade ${updgradeflags}
            ;;

        "--qemu") # installs QEMU/KVM
            update ${updateflags}
            apt install -y qemu-kvm libvirt-clients libvirt-daemon-system bridge-utils virtinst libvirt-daemon
            DEBIAN_FRONTEND=noninteractive apt install -y --only-upgrade qemu-block-extra qemu-system-common qemu-system-data qemu-system-gui qemu-system-x86 qemu-utils tzdata
            upgrade ${updgradeflags}
            ;;

        "--virt-manger") # installs virt-manager
            update ${updateflags}
            apt install -y virt-manager
            upgrade ${updgradeflags}
            ;;

        "--enable-libvirtd") # enables libvirtd
            update ${updateflags}
            systemctl enable --now libvirtd
            upgrade ${updgradeflags}
            ;;

        "--rustc") # installs rustc
            update ${updateflags}
            apt install -y --only-upgrade gdm3 gir1.2-gdm-1.0 libgdm1 qemu-block-extra qemu-system-common qemu-system-data qemu-system-gui qemu-system-x86 qemu-utils
            apt install -y cargo pkg-config fontconfig
            apt install -y libfontconfig1-dev libfontconfig
            upgrade ${updgradeflags}
            ;;

        "--configuration") # installs keyboard-configuration
            update ${updateflags}
            DEBIAN_FRONTEND=noninteractive apt-get install -y keyboard-configuration gettext-base
            apt install -y --no-install-recommends curl ca-certificates git build-essential lldb lcov cmake clangd g++ gcc gdb lcov ninja-build pkg-config
            apt install -y --only-upgrade distro-info-data
            upgrade ${updgradeflags}
            ;;


        "--llvm") # installs llvm and llvm-cov
            update ${updateflags}
            apt install -y llvm
            upgrade ${updgradeflags}
            ;;

        "--zsh") # installs zsh
            update ${updateflags}
            apt install -y zsh
            chsh -s $(which bash)
            upgrade ${updgradeflags}
            ;;

        "--bazel") # installs bazel
            update ${updateflags}
            apt install -y apt-transport-https curl gnupg
            curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --batch --yes --dearmor >bazel-archive-keyring.gpg
            mv bazel-archive-keyring.gpg /usr/share/keyrings
            echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list
            apt install -y bazel
            upgrade ${updgradeflags}
            ;;

        "--gh") # installs gh
            update ${updateflags}
            type -p curl >/dev/null || apt install curl -y
            curl --silent -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | dd of=/usr/shar\(e/keyrings/githubcli-archive-keyring.gpg
            chmod go+r /usr/share/keyrings/githubcli-archive-keyring.gpg
            echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | tee /etc/apt/sources.list.d/github-cli.list >/dev/null
            apt install -y gh
            upgrade ${updgradeflags}
            ;;

        "--git") # installs git
            update ${updateflags}
            apt install -y git
            upgrade ${updgradeflags}
            ;;

        "--colordiff") # installs colordiff
            update ${updateflags}
            apt install -y colordiff
            upgrade ${updgradeflags}
            ;;

        "--meld") # installs meld
            update ${updateflags}
            apt install -y meld
            upgrade ${updgradeflags}
            ;;

        "--sublime-merge") # installs sublime-merge
            update ${updateflags}
            apt install -y apt-transport-https
            wget -qO - https://download.sublimetext.com/sublimehq-pub.gpg | gpg --batch --yes --dearmor | tee /etc/apt/trusted.gpg.d/sublimehq-archive.gpg >/dev/null
            echo "deb https://download.sublimetext.com/ apt/stable/" | tee /etc/apt/sources.list.d/sublime-text.list
            apt update -y
            apt install -y sublime-merge
            upgrade ${updgradeflags}
            ;;

        "--pyenv") # installs pyenv
            update ${updateflags}
            apt install -y build-essential zlib1g-dev libffi-dev libssl-dev libbz2-dev libreadline-dev libsqlite3-dev liblzma-dev python3-tk tk-dev
            apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire libpipewire-0.3-0 libpipewire-0.3-common libpipewire-0.3-modules open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
            upgrade ${updgradeflags}
            ;;

        "--python") # installs python
            update ${updateflags}
            apt install -y build-essential curl git ca-certificates python3 python3-dev python3-pip python3-venv python3-behave python3-virtualenv
            apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire libpipewire-0.3-0 libpipewire-0.3-common libpipewire-0.3-modules open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
            upgrade ${updgradeflags}
            ;;

        "--clang-lldb-mi") # installs lldb-mi dependencies
            update ${updateflags}
            apt-get install -y libclang-dev liblldb-dev || apt-get install libclang-6.0-dev liblldb-6.0-dev || apt-get install libclang-4.0-dev liblldb-4.0-dev || apt-get install libclang-3.8-dev liblldb-3.8-dev
            apt install python3-lldb-14
            ln -s /usr/lib/llvm-14/lib/python3.10/dist-packages/lldb/* /usr/lib/python3/dist-packages/lldb/
            upgrade ${updgradeflags}
            ;;

        "--clang-format") # installs clang-format
            update ${updateflags}
            apt install -y clang-format
            apt install -y apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire  open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
            apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire  open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
            upgrade ${updgradeflags}
            ;;

        "--cmake") # installs cmake
            update ${updateflags}
            apt install -y --no-install-recommends valgrind curl ca-certificates git build-essential lldb lcov cmake clangd clang-format g++ gcc gdb lcov ninja-build bison
            upgrade ${updgradeflags}
            ;;

        "--docker") # installs docker
            update ${updateflags}
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
            usermod -a -G sudo $USER
            if [ "$(cat /etc/group | grep -s '^docker:')" == "" ]; then
                groupadd docker
                usermod -aG docker $USER
            fi
            chmod 666 /var/run/docker.sock
            upgrade ${updgradeflags}
            ;;

        "--docker-compose") # installs docker-compose
            update ${updateflags}
            apt install -y \
                desktop-file-utils
            apt install -y --fix-broken qemu-kvm libvirt-clients libvirt-daemon-system bridge-utils virtinst libvirt-daemon pass uidmap
            systemctl enable --now libvirtd
            curl --silent -L https://desktop.docker.com/linux/main/amd64/docker-desktop-4.15.0-amd64.deb -o /tmp/docker-desktop-4.15.0-amd64.deb
            apt update -y
            dpkg -i /tmp/docker-desktop-4.15.0-amd64.deb
            apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report gnome-remote-desktop grub-common grub-pc grub-pc-bin grub2-common gstreamer1.0-pipewire libpipewire-0.3-0 libpipewire-0.3-common libpipewire-0.3-modules open-vm-tools open-vm-tools-desktop python3-software-properties software-properties-common software-properties-gtk libgbm1 libgl1-mesa-dri libglapi-mesa libglx-mesa0 mesa-va-drivers mesa-vulkan-drivers
            upgrade ${updgradeflags}
            ;;

        "--gh-cli") # installs github cli
            update ${updateflags}
            [[ ! -d "${pwd}/gh" ]] && mkdir "${pwd}/gh"
            curl -L "https://github.com/cli/cli/releases/download/v2.22.0/gh_2.22.0_linux_amd64.deb" -o "${pwd}/gh/gh_2.22.0_linux_amd64.deb"
            dpkg -i "${pwd}/gh/gh_2.22.0_linux_amd64.deb"
            ;;

        "--help") # [optional] shows command description
            help
            ;;

        *)
            help
            ;;

    esac
    shift
done

[[ $SHLVL -gt 2 ]] || echo OK

cd "${pwd}"
