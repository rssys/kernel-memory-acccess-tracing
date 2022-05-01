#!/bin/sh

mkdir -p build
cd build && cmake .. -DLLVM_BUILD_PATH=/usr/lib/llvm-12/build/ && make -j`nproc`