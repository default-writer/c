# Update the VARIANT arg in devcontainer.json to pick an Ubuntu version: focal (or ubuntu-20.04), bionic (or ubuntu-18.04)
ARG VARIANT="focal"
FROM buildpack-deps:${VARIANT}-curl

# This Dockerfile adds a non-root user with sudo access. Update the "remoteUser" property in
# devcontainer.json to use it. More info: https://aka.ms/vscode-remote/containers/non-root-user.
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Options for common setup script - SHA generated on release
ARG INSTALL_ZSH="true"
ARG UPGRADE_PACKAGES="true"
ARG COMMON_SCRIPT_SOURCE="https://raw.githubusercontent.com/microsoft/vscode-dev-containers/master/script-library/common-debian.sh"
ARG COMMON_SCRIPT_SHA="dev-mode"

# Install needed packages and setup non-root user. Use a separate RUN statement to add your own dependencies.
RUN apt-get update \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends curl ca-certificates 2>&1 \
    && apt-get -y install build-essential gdb \
    && curl -sSL  ${COMMON_SCRIPT_SOURCE} -o /tmp/common-setup.sh \
    && ([ "${COMMON_SCRIPT_SHA}" = "dev-mode" ] || (echo "${COMMON_SCRIPT_SHA} /tmp/common-setup.sh" | sha256sum -c -)) \
    && /bin/bash /tmp/common-setup.sh "${INSTALL_ZSH}" "${USERNAME}" "${USER_UID}" "${USER_GID}" "${UPGRADE_PACKAGES}" \
    # Clean up
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/* /tmp/common-setup.sh

###
### General devlopment
###
RUN apt-get update \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends build-essential tar curl zip unzip g++

###
### Node.js
###
RUN curl -sL https://deb.nodesource.com/setup_14.x -o /tmp/nodesource_setup.sh \
    && chmod +x /tmp/nodesource_setup.sh \
    && /tmp/nodesource_setup.sh && rm /tmp/nodesource_setup.sh \
    && apt install nodejs

###
### CMake
###
RUN wget -qO- "https://cmake.org/files/v3.18/cmake-3.18.1-Linux-x86_64.tar.gz" | tar --strip-components=1 -xz -C /usr/local

###
### Go
###
RUN wget -qO- "https://golang.org/dl/go1.15.linux-amd64.tar.gz" | tar --strip-components=1 -xz -C /usr/local

###
### Bazel
###
RUN apt install curl gnupg \
    && curl https://bazel.build/bazel-release.pub.gpg | apt-key add -; \
    echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list \
    && apt-get update \
    && apt-get -y install bazel \
    && wget -q "https://github.com/bazelbuild/buildtools/releases/download/3.4.0/buildifier" -P /usr/local/bin &&  chmod +x /usr/local/bin/buildifier \
    && wget -q "https://github.com/bazelbuild/buildtools/releases/download/3.4.0/buildozer" -P /usr/local/bin && chmod +x /usr/local/bin/buildozer \
    && wget -q "https://github.com/bazelbuild/buildtools/releases/download/3.4.0/unused_deps" -P /usr/local/bin && chmod +x /usr/local/bin/unused_deps

###
### LLDB
###
RUN apt-get install gnupg lldb g++ valgrind -y 
