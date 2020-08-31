add_executable(helloworld main.cpp)
target_include_directories(helloworld PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)

if(WIN32)
  target_link_libraries(helloworld wsock32 ws2_32)
endif()
