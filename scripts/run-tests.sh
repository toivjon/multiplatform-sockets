#!/bin/bash
set -ev
if [ $TRAVIS_OS_NAME = "linux" ]; then
  cppcheck -q -I include -i build -i examples --enable=all --suppress=missingIncludeSystem --error-exitcode=1 .
  clang-tidy -checks=* ./include/mps.h -- -Wall -std=c++11 -x c++
fi
cmake -S . -B build
cmake --build build
cd build
ctest
