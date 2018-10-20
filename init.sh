#!/usr/bin/env sh
mkdir -p build && cd build

# Change the -G option according to your build system, might be different for example if you were using MINGW
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-toolchain.cmake -G "CodeBlocks - Unix Makefiles" ../

