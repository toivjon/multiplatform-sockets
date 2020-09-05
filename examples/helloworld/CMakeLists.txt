add_executable(helloworld main.cpp)
target_include_directories(helloworld PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
