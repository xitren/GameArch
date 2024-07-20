FROM alpine:3.18 AS linux-base
ENV DEBIAN_FRONTEND noninteractive
SHELL ["/bin/sh", "-o", "pipefail", "-c"]

FROM linux-base AS code-check-base
RUN apk update 
RUN apk upgrade
RUN apk --no-cache add clang bash gtest-dev cpputest git cmake make ninja-build python3 perl-file-pushd musl-dev spdlog-dev
RUN apk --no-cache add clang-extra-tools patch
RUN apk --no-cache add "clang16-extra-tools==16.0.6-r1"
RUN apk --no-cache add "gcc-arm-none-eabi==12.2.0-r8"
RUN apk --no-cache add "g++-arm-none-eabi==12.2.0-r1"
RUN apk --no-cache add "gcc==12.2.1_git20220924-r10"
RUN apk --no-cache add "g++==12.2.1_git20220924-r10"
RUN apk --no-cache add grpc protobuf-dev

RUN ln -sf python3 /usr/bin/python
RUN ln -s /usr/bin/gcc /usr/bin/gcc-12
RUN ln -s /usr/bin/g++ /usr/bin/g++-12
RUN ln -s /usr/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc-12
RUN ln -s /usr/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++-12

RUN python3 -m ensurepip
RUN pip3 install --no-cache --upgrade pip setuptools
RUN pip3 install --upgrade google-api-python-client
RUN pip3 install grpcio==1.54.2
RUN pip3 install grpcio-tools==1.54.2
RUN pip3 install protobuf==4.23.1

CMD ["uname", "-a"]