add_executable(broadcast_receiver main.cpp)
target_include_directories(broadcast_receiver PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
