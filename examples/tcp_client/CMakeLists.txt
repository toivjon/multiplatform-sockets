add_executable(tcp_client main.cpp)
target_include_directories(tcp_client PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
