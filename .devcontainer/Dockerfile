FROM ubuntu:22.04 as base

RUN apt-get update && \
    export DEBIAN_FRONTEND=noninteractive && \
    apt-get update && \ 
    apt-get -y upgrade && \ 
    apt-get -y install --no-install-recommends build-essential lldb lcov cmake clangd g++ gcc

RUN apt-get autoclean -y && \
    apt-get autoremove -y && \
    rm -rf /var/lib/{cache,log}/ && \
    rm -rf /var/lib/apt/lists/*.lz4 && \
    rm -rf /tmp/* /var/tmp/* && \
    rm -rf /usr/share/doc/ && \
    rm -rf /usr/share/man/
