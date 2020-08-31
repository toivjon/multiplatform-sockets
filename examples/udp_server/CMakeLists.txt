add_executable(udp_server main.cpp)
target_include_directories(udp_server PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)

if(WIN32)
  target_link_libraries(udp_server wsock32 ws2_32)
endif()
