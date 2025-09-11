FROM ubuntu:24.04

ENV TZ=Europe/Moscow

RUN ln -snf /usr/share/zoneinfo/"$TZ" /etc/localtime && echo "$TZ" > /etc/timezone && \
    apt -y update && \
    export DEBIAN_FRONTEND=noninteractive && \
    apt -y install --no-install-recommends -y git ca-certificates gcc g++ make cmake ninja-build lcov  && \
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

# Copy all source files needed to run the initialization
COPY bin /workspace/bin
