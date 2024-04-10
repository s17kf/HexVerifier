#!/bin/bash


cd "$(dirname "$0")"

cmake --build ../cmake-build-debug --target all -j 6

valgrind ./../cmake-build-debug/UtilsTests
valgrind ./../cmake-build-debug/DataStructuresTests
valgrind ./../cmake-build-debug/BoardTests
