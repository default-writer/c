FROM ubuntu:24.04

WORKDIR /workspace

COPY . .

RUN /workspace/bin/init.sh --init --lcov --setup --optional
