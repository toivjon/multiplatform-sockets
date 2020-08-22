add_library(mps "socket.cpp"  "wsa.cpp"  "exception.cpp" "error.h" "error.cpp" "udp_socket.cpp" "address.cpp" "udp_packet.cpp" "tcp_socket.cpp" "tcp_server_socket.cpp" "tcp_client_socket.cpp")

target_include_directories(mps
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
)
