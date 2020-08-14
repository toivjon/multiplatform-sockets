#include "mps/tcp_socket.h"
#include "mps/udp_socket.h"

using namespace mps;

int main() {
  TCPSocket tcpSocket(AddressFamily::IPv4);
  UDPSocket udpSocket(AddressFamily::IPv6);
  tcpSocket.bind(Address("127.0.0.1", 5555));
  udpSocket.bind(Address("::1", 5556));
  udpSocket.sendTo(Address("127.0.0.1", 5556), reinterpret_cast<const void*>("hello"), 5);

	return 0;
}
