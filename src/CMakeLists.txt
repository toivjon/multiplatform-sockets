add_library(mps "wsa.cpp"  "exception.cpp" "error.h" "error.cpp" "udp_socket.cpp" "address.cpp" "udp_packet.cpp" "tcp_socket.cpp" "tcp_server_socket.cpp" "tcp_client_socket.cpp" "socket.cpp" "internal.h")

target_include_directories(mps
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
)
