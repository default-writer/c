FROM ubuntu:22.04

ARG USER_NAME=vscode
ARG USER_GROUP=vscode
ARG USER_ID=1000
ARG USER_GID=$USER_ID

# Options for common setup script - SHA generated on release
ARG INSTALL_ZSH="true"
ARG UPGRADE_PACKAGES="true"
ARG COMMON_SCRIPT_SOURCE="https://raw.githubusercontent.com/microsoft/vscode-dev-containers/master/script-library/common-debian.sh"
ARG COMMON_SCRIPT_SHA="dev-mode"

ENV TZ=Europe/Istanbul
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install needed packages and setup non-root user. Use a separate RUN statement to add your own dependencies.
RUN apt -y update && \
    export DEBIAN_FRONTEND=noninteractive && \
    apt -y install --no-install-recommends curl ca-certificates git build-essential lldb lcov cmake clangd g++ gcc gdb lcov ninja-build gettext-base && \
    curl -sSL  ${COMMON_SCRIPT_SOURCE} -o /tmp/common-setup.sh && \
    ([ "${COMMON_SCRIPT_SHA}" = "dev-mode" ] || (echo "${COMMON_SCRIPT_SHA} /tmp/common-setup.sh" | sha256sum -c -)) && \
    /bin/bash /tmp/common-setup.sh "${INSTALL_ZSH}" "${USER_NAME}" "${USER_ID}" "${USER_GID}" "${UPGRADE_PACKAGES}" && \
    apt -y upgrade && \
    # Clean up
    apt autoclean -y && \
    apt autoremove -y && \
    rm -rf /var/lib/{cache,log}/ && \
    rm -rf /tmp/* /var/tmp/* && \
    rm -rf /usr/share/doc/ && \
    rm -rf /usr/share/man/ && \
    rm -rf /var/lib/apt/lists/* /tmp/common-setup.sh

RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo

USER ${USER_NAME}
WORKDIR /workspace

COPY --chown=${USER_NAME}:${USER_GROUP} . .