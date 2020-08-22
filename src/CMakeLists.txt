add_library(mps "socket.cpp" "tcp_socket.cpp" "wsa.cpp"  "exception.cpp" "error.h" "error.cpp" "new_udp_socket.cpp" "address.cpp" "udp_packet.cpp")

target_include_directories(mps
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
)
