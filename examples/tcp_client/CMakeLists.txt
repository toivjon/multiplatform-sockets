add_executable(tcp_client main.cpp)
target_include_directories(tcp_client PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)

if(WIN32)
  target_link_libraries(tcp_client wsock32 ws2_32)
endif()
