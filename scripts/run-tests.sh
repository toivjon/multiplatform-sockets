#!/bin/bash
set -ev
if [ $TRAVIS_OS_NAME = "linux" ]; then
  cppcheck -q -I include -i build -i examples --enable=all --suppress=missingIncludeSystem --error-exitcode=1 .
  clang-tidy ./include/mps.h -checks=*
fi
cmake -S . -B build
cmake --build build
cd build
ctest
