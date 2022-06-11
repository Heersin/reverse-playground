#!/bin/bash
CDIR=$(pwd)
# "CodeBlocks - Unix Makefiles"

cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=make -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang -S $CDIR -B $CDIR/cmake-build-debug &&
	cmake --build cmake-build-debug --target bing2yi &&
	cp cmake-build-debug/bing2yi tired_vm &&
	strip tired_vm

echo "[DONE] buidl in $CDIR/tired_vm"
