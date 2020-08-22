add_executable(tcp_server main.cpp)
target_include_directories(tcp_server  PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
target_link_libraries(tcp_server mps)

if(WIN32)
  target_link_libraries(tcp_server wsock32 ws2_32)
endif()
