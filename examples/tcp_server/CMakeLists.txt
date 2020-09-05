add_executable(tcp_server main.cpp)
target_include_directories(tcp_server  PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
