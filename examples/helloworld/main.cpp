#include "mps/tcp_socket.h"
#include "mps/udp_socket.h"

using namespace mps;

int main() {
  TCPSocket tcpSocket(AddressFamily::IPv4);
  UDPSocket udpSocket(AddressFamily::IPv6);
	return 0;
}
