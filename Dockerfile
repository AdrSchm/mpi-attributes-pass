FROM ubuntu:20.04

WORKDIR /opt

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y && apt-get upgrade -y && apt-get install -y gcc g++ gdb cmake cmake-curses-gui python3 apt-utils wget software-properties-common gnupg git autoconf automake libtool zlib1g-dev zlib1g vim unzip python3-pip python3-pytest python3-pytest-cov openmpi-bin openmpi-common bison flex python2

RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
RUN add-apt-repository 'deb http://apt.llvm.org/focal/ llvm-toolchain-focal-17 main'
#RUN add-apt-repository 'deb-src http://apt.llvm.org/focal/ llvm-toolchain-focal-17 main'
RUN apt update

RUN apt-get install -y libllvm-17-ocaml-dev libllvm17 llvm-17 llvm-17-dev llvm-17-doc llvm-17-examples llvm-17-runtime clang-17 clang-tools-17 clang-17-doc libclang-common-17-dev libclang-17-dev libclang1-17 clang-format-17 python3-clang-17 clangd-17 clang-tidy-17

RUN ln -s /usr/bin/clang-17 /usr/bin/clang && ln -s /usr/bin/clang++-17 /usr/bin/clang++ && ln -s /usr/lib/llvm-17/build/utils/lit/lit.py /usr/bin/lit && ln -s /usr/bin/opt-17 /usr/bin/opt && ln -s /usr/bin/FileCheck-17 /usr/bin/FileCheck && ln -s /usr/bin/llvm-objdump-17 /usr/bin/llvm-objdump
