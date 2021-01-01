#!/usr/bin/env bash
set -Eeuo pipefail

if ! [ "$(id -u)" = 0 ]; then
    echo "Please run this script with superuser access!"
    exit 1
fi
set -ex

VERSION=${VERSION:-"7.0.1"}

script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd -P)

cleanup() {
	rm "${script_dir}/fmt-$VERSION.tgz"
	rm -rf "${script_dir}/fmt-$VERSION"
}
trap cleanup SIGINT SIGTERM ERR EXIT

unamestr=$(uname)
if [ ! -f fmtlib-$VERSION.tgz ]; then
	wget https://github.com/fmtlib/fmt/archive/$VERSION.tar.gz -O fmt-$VERSION.tgz
fi
tar -xzvf fmt-$VERSION.tgz
cd fmt-$VERSION
if [ -d build ]; then
	rm -R build
fi
mkdir build

# Should install in /usr/local
cd build && \
cmake .. && \
make -j$(nproc) && \
make install
