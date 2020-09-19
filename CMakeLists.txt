cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

# global project configuration.
project(MPS VERSION 1.0.0 LANGUAGES CXX)

# include CMake framework unit testing environment.
include(CTest)

# the list of submodules.
add_subdirectory(examples)
add_subdirectory(tests)
