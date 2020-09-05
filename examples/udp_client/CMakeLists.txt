add_executable(udp_client main.cpp)
target_include_directories(udp_client PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
