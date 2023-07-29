FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu-22.04

COPY meta.env /usr/local/etc/vscode-dev-containers

# strange issue: https://stackoverflow.com/questions/67732260/how-to-fix-hash-sum-mismatch-in-docker-on-mac
RUN rm -rf /var/lib/apt/lists/*
RUN apt-get clean
RUN echo    "Acquire::http::Pipeline-Depth    0;" > /etc/apt/apt.conf.d/99custom \
    && echo "Acquire::http::No-Cache          true;" >> /etc/apt/apt.conf.d/99custom \
    && echo "Acquire::CompressionTypes::Order gz;" >> /etc/apt/apt.conf.d/99custom \
    && echo "Acquire::BrokenProxy             true;" >> /etc/apt/apt.conf.d/99custom

# common packages
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y ca-certificates gpg gnupg wget software-properties-common lsb-release doxygen python3-pip \
    && python3 -m pip install --upgrade pip cmakelang \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

# latest stable g++ and gcc
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test \
    && apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && version=$(apt-cache pkgnames | sed -nr 's/^gcc-(([0-9]+\.?)+)$/\1/p' | sort -n | tail -n1) \
    && apt-get install -y gcc g++ gcc-$version g++-$version \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-$version 60 --slave /usr/bin/g++ g++ /usr/bin/g++-$version \
    && update-alternatives --install /usr/bin/cc cc /usr/bin/gcc-$version 60 \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

# latest stable llvm
COPY --chown=root llvm.sh llvm.sh
RUN ./llvm.sh all \
    && rm llvm.sh

# latest stable cmake
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null \
    && echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null \
    && apt-get update \
    && rm /usr/share/keyrings/kitware-archive-keyring.gpg \
    && apt-get install -y kitware-archive-keyring cmake \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

ARG USERNAME=vscode
