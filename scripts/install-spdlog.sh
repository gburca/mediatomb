#!/usr/bin/env bash
set -Eeuo pipefail

if ! [ "$(id -u)" = 0 ]; then
    echo "Please run this script with superuser access!"
    exit 1
fi
set -ex

VERSION=${VERSION:-"1.7.0"}

script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd -P)

cleanup() {
	rm "${script_dir}/spdlog-$VERSION.tgz"
	rm -rf "${script_dir}/spdlog-$VERSION"
}
trap cleanup SIGINT SIGTERM ERR EXIT

unamestr=$(uname)
if [ ! -f spdlog-$VERSION.tgz ]; then
	wget https://github.com/gabime/spdlog/archive/v$VERSION.tar.gz -O spdlog-$VERSION.tgz
fi
tar -xzvf spdlog-$VERSION.tgz
cd spdlog-$VERSION
if [ -d build ]; then
	rm -R build
fi
mkdir build
cd build && \
cmake .. -DSPDLOG_FMT_EXTERNAL=ON && \
make spdlog && \
make install/fast
