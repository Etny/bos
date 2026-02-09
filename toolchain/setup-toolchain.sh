#!/bin/bash

parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
mkdir -p $parent_path/build/gcc
mkdir -p $parent_path/build/binutils
mkdir -p $parent_path/bin 
cd $parent_path/build
export PREFIX="$parent_path"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

curl https://ftp.gnu.org/gnu/binutils/binutils-2.45.1.tar.xz | tar -xJ
curl https://ftp.gnu.org/gnu/gcc/gcc-15.2.0/gcc-15.2.0.tar.xz | tar -xJ

cd $parent_path/build/binutils
../binutils-2.45.1/configure --target=$TARGET --prefix="$PREFIX" \
    --with-sysroot --disable-nls --disable-werror
make -j
make install


cd $parent_path/build/gcc
../gcc-15.2.0/configure --target=$TARGET --prefix="$PREFIX" \
    --disable-nls --enable-languages=c --without-headers \
    --disable-hosted-libstdcxx
make all-gcc
make all-target-libgcc
make all-target-libstdc++-v3
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3
