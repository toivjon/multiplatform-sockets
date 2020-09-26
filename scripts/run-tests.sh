#!/bin/bash
set -ev
cmake -S . -B build
cmake --build build
cd build
ctest

if [ $TRAVIS_OS_NAME = "linux" ]; then
  cd ..
  cppcheck -q -I include -i build -i examples --enable=all --suppress=missingIncludeSystem --error-exitcode=1 . 
fi
