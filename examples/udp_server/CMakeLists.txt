add_executable(udp_server main.cpp)
target_include_directories(udp_server PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
