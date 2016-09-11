#!/bin/sh
cmake -DCMAKE_TOOLCHAIN_FILE="$KL25Z_SDK_DIR/tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug  .
make -j4
cmake -DCMAKE_TOOLCHAIN_FILE="$KL25Z_SDK_DIR/tools/cmake_toolchain_files/armgcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release  .
make -j4
