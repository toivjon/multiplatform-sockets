add_library(mps socket.cpp "tcp_socket.cpp" "udp_socket.cpp")

target_include_directories(mps
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
)
