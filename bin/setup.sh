#!/usr/bin/env bash
set -e

uid=$(id -u)

if [ ! "${uid}" -eq 0 ]; then
	echo "Please run as root"
	exit
fi

pwd=$(pwd)

install="$1"

# you can use command `apt install --only-upgrade`` for packages wouldn't upgrade since they are marked as manual installed packages
if [ "${install}" == "kvm" ]; then
	apt update -y
	apt-get install -y qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils
	adduser `id -un` libvirt --quiet
	adduser `id -un` kvm --quiet
	apt upgrade -y
fi

if [ "${install}" == "gh" ]; then
	apt update -y
	type -p curl >/dev/null || apt install curl -y
	curl --silent -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | dd of=/usr/share/keyrings/githubcli-archive-keyring.gpg \
	&& chmod go+r /usr/share/keyrings/githubcli-archive-keyring.gpg \
	&& echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | tee /etc/apt/sources.list.d/github-cli.list > /dev/null \
	&& apt update -y \
	&& apt install gh -y
	apt upgrade -y
fi

if [ "${install}" == "git" ]; then
	apt update -y
	apt install -y git
	apt upgrade -y
fi

if [ "${install}" == "pyenv" ]; then
	apt update -y
	apt install -y build-essential zlib1g-dev libffi-dev libssl-dev libbz2-dev libreadline-dev libsqlite3-dev liblzma-dev python3-tk tk-dev
	apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report
	apt upgrade -y
fi

if [ "${install}" == "python" ]; then
	apt update -y
	apt install -y build-essential curl git ca-certificates python3 python3-dev python3-pip python3-venv python3-behave python3-virtualenv
	apt install -y --only-upgrade apport apport-gtk python3-apport python3-problem-report
	apt upgrade -y
fi

if [ "${install}" == "cmake" ]; then
	apt update -y
	apt install -y --no-install-recommends curl ca-certificates git build-essential lldb lcov cmake clangd g++ gcc gdb lcov ninja-build
	apt upgrade -y
fi

if [ "${install}" == "docker" ]; then
	apt update -y
	apt install -y \
		ca-certificates \
		curl \
		gnupg \
		lsb-release
	mkdir -p /etc/apt/keyrings
	curl --silent -fsSL --use-ascii --retry 5 --retry-all-errors https://download.docker.com/linux/ubuntu/gpg | gpg --dearmor -o /etc/apt/keyrings/docker.gpg
	echo \
		"deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) stable" | tee /etc/apt/sources.list.d/docker.list >/dev/null

	apt update -y
	chmod a+r /etc/apt/keyrings/docker.gpg

	apt update -y
	apt install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin
	apt upgrade -y
fi

cd "${pwd}"
