cmake_minimum_required(VERSION 3.11)

include(FetchContent)
include(GoogleTest)

# specify where we can download Google unit testing framework.
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)

# ensure that the Google unit test framework gets loaded.
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)

  # prevent GoogleTest to override VS options.
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  # prevent GoogleTest from using PThreads.
  set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)

  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

# specify the files included in the test binaries.
add_executable(mps_test ip_address_test.cpp)

# define which libraries we want to link with our unit tests.
target_link_libraries(mps_test PRIVATE gtest_main)

# specify the location of public headers.
target_include_directories(mps_test PUBLIC ${PROJECT_SOURCE_DIR}/include)

# use GooglTest module auto-discover to find tests.
gtest_discover_tests(mps_test)
