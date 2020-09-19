cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

# global project configuration.
project(MPS VERSION 1.0.0 LANGUAGES CXX)

# include CMake framework unit testing environment.
include(CTest)

# specify the C++ version (C++11) we are using in this project.
set(CMAKE_CXX_STANDARD 11)

# the list of submodules.
add_subdirectory(examples)
add_subdirectory(tests)
