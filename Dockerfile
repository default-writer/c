FROM ubuntu:24.04

# ENV TZ=Europe/Istanbul

# # Install sudo and other necessary packages
RUN apt -y update && \
    export DEBIAN_FRONTEND=noninteractive &&  \
    apt -y install --no-install-recommends -y git cmake lcov && \
    apt -y upgrade && \
    apt autoclean -y && \
    apt autoremove -y && \
    rm -rf /var/lib/{cache,log}/ && \
    rm -rf /tmp/* /var/tmp/* && \
    rm -rf /usr/share/doc/ && \
    rm -rf /usr/share/man/ && \    
    rm -rf /var/lib/apt/lists/*

# RUN ln -snf /usr/share/zoneinfo/"$TZ" /etc/localtime && echo "$TZ" > /etc/timezone && \
#     apt -y update && \
#     export DEBIAN_FRONTEND=noninteractive && \
#     apt -y install --no-install-recommends -y \
#     build-essential \
#     ca-certificates \
#     nodejs \
#     npm \
#     python3 \
#     python3-venv \
#     python3-virtualenv \
#     clangd \
#     cmake \
#     curl \
#     g++ \
#     gcc \
#     gdb \
#     gettext-base \
#     git \
#     gnupg \
#     lcov \
#     lldb \
#     lsb-release \
#     make \
#     ninja-build \
#     software-properties-common \
#     sudo \
#     wget && \
#     apt -y upgrade && \
#     apt autoclean -y && \
#     apt autoremove -y && \
#     rm -rf /var/lib/{cache,log}/ && \
#     rm -rf /tmp/* /var/tmp/* && \
#     rm -rf /usr/share/doc/ && \
#     rm -rf /usr/share/man/ && \
#     rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /workspace

# Copy all source files needed to run the initialization
COPY bin /workspace/bin

# Run the initialization script
#RUN ./bin/init.sh --init --lcov  --setup
