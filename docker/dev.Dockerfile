FROM ubuntu:jammy AS ubuntu-base
ENV DEBIAN_FRONTEND noninteractive
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

USER root
RUN apt-get -qq update
RUN apt-get -qq upgrade

RUN apt-get -qq --no-install-recommends install apt-utils bzr wget xz-utils unzip
RUN apt-get -qq --no-install-recommends install vim-tiny
RUN apt-get -qq --no-install-recommends install sudo
RUN apt-get -qq --no-install-recommends install gcc-12 g++-12
RUN apt-get -qq --no-install-recommends install cmake make ninja-build
RUN apt-get -qq --no-install-recommends install libgtest-dev
RUN apt-get -qq --no-install-recommends install clang-format-14
RUN apt-get -qq --no-install-recommends install cmake-format
RUN apt-get -qq --no-install-recommends install clang-tidy-15
RUN apt-get -qq --no-install-recommends install git
RUN apt-get -qq --no-install-recommends install build-essential

RUN apt-get -qq --no-install-recommends install libspdlog-dev libfmt-dev
RUN apt-get -qq --no-install-recommends install libboost-all-dev
RUN apt-get -qq --no-install-recommends install libopencv-dev python3-opencv
RUN apt-get -qq --no-install-recommends install python3-pip
RUN apt-get -qq --no-install-recommends install nlohmann-json3-dev
RUN apt-get -qq --no-install-recommends install gdb

RUN apt-get -qq --no-install-recommends install curl
RUN apt-get -qq --no-install-recommends install libtool libssl-dev
RUN apt-get -qq --no-install-recommends install pkg-config
RUN apt-get -qq --no-install-recommends install software-properties-common
RUN apt-get -qq --no-install-recommends install meson
RUN apt-get -qq --no-install-recommends install libyaml-dev python3-yaml python3-ply python3-jinja2
RUN apt-get -qq --no-install-recommends install g++ clang
RUN apt-get -qq --no-install-recommends install libdrm-dev libexif-dev
RUN apt-get -qq --no-install-recommends install ccache
RUN apt-get -qq --no-install-recommends install autoconf
RUN apt-get -qq --no-install-recommends install protobuf-compiler
RUN apt-get -qq --no-install-recommends install libprotobuf-dev

RUN pip3 install --upgrade google-api-python-client
RUN pip3 install grpcio==1.54.2
RUN pip3 install grpcio-tools==1.54.2
RUN pip3 install protobuf==4.23.1

RUN apt-get -qq clean
RUN rm -rf /var/lib/apt/lists/*

# install gRPC
RUN useradd -ms /bin/bash user
USER user
RUN mkdir -p /home/user/scripts
COPY --chown=user:user --chmod=777 scripts/install_grpc.sh /home/user/scripts/install_grpc.sh
USER root
RUN /home/user/scripts/install_grpc.sh
