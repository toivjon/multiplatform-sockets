#!/bin/bash
set -ev
cmake -S . -B build
cmake --build build
cd build
ctest
