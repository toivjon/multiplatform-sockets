add_executable(broadcast_sender main.cpp)
target_include_directories(broadcast_sender PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
