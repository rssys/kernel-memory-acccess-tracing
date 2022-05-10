#! /bin/bash

cd gcc
mkdir build
cd build
$PWD/../configure --prefix=$PWD/../../gcc-memtrace --enable-languages=c,c++
make -j`nproc`
make install
