FROM ubuntu:24.04

ENV TZ=Europe/Istanbul

# Install sudo and other necessary packages
RUN ln -snf /usr/share/zoneinfo/"$TZ" /etc/localtime && echo "$TZ" > /etc/timezone && \
    apt -y update && \
    export DEBIAN_FRONTEND=noninteractive && \
    apt -y install --no-install-recommends -y \
    build-essential \
    ca-certificates \
    clangd \
    cmake \
    curl \
    g++ \
    gcc \
    gdb \
    gettext-base \
    git \
    gnupg \
    lcov \
    lldb \
    lsb-release \
    make \
    ninja-build \
    software-properties-common \
    sudo \
    wget && \
    apt -y upgrade && \
    apt autoclean -y && \
    apt autoremove -y && \
    rm -rf /var/lib/{cache,log}/ && \
    rm -rf /tmp/* /var/tmp/* && \
    rm -rf /usr/share/doc/ && \
    rm -rf /usr/share/man/ && \
    rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /workspace

# Copy the contents of the current directory to /workspace
COPY . .

# Run the initialization script
RUN /workspace/bin/init.sh --init --lcov --setup

RUN rm -rf /workspace
