#!/bin/bash
# This variable will be empty if running in docker env, not otherwise
NOT_RUNNING_IN_DOCKER=$(cat /proc/1/sched | head -n 1 | grep -E 'systemd|init')

# Only run sudo if executed locally
SUDO=''
if [[ -n $NOT_RUNNING_IN_DOCKER ]]; then
    echo "Not running in Docker"
    SUDO='sudo'
else
	echo "Running in Docker"
fi

# Run with nproc-1 unless there is only one processor
NPROC=1
if [ "$(nproc)" > 1 ]; then
    NPROC="$(($(nproc) - 1))"
fi
echo "Building with $NPROC processor(s)"

# Set environmental variables to install gRPC (if not in docker container, install locally in .local folder)
if [[ -n $NOT_RUNNING_IN_DOCKER ]]; then
    MY_INSTALL_DIR=$HOME/.local
else
    MY_INSTALL_DIR=/usr/local
fi

echo "gRPC install directory: ${MY_INSTALL_DIR}"

mkdir -p $MY_INSTALL_DIR
export PATH="$MY_INSTALL_DIR/bin:$PATH"

# Clone Repository
git clone --recurse-submodules -b v1.45.0 https://github.com/grpc/grpc

# Build and install grpc
cd grpc
mkdir -p cmake/build
pushd cmake/build
    cmake -DgRPC_INSTALL=ON \
        -DgRPC_BUILD_TESTS=OFF \
        -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
        ../..
    make -j$NPROC
    make install
popd

# Remove repository after installation
cd ..
$SUDO rm -r grpc
