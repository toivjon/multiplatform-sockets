#include "mps/tcp_socket.h"
#include "mps/udp_socket.h"
#include "mps/exception.h"

#include <iostream>

using namespace mps;

int main() {
  try {
    UDPSocket socket;
    std::cout << "1 Bound to port: " << socket.getAddress().getPort() << std::endl;
    std::cout << "1 Bound to host: " << socket.getAddress().getIPAddress() << std::endl;
    UDPSocket socket2(56789);
    std::cout << "2 Bound to port: " << socket2.getAddress().getPort() << std::endl;
    std::cout << "2 Bound to host: " << socket2.getAddress().getIPAddress() << std::endl;
    UDPSocket socket3({ UDPSocket::Flag::IPv6, UDPSocket::Flag::Broadcast });
    std::cout << "3 Bound to port: " << socket3.getAddress().getPort() << std::endl;
    std::cout << "3 Bound to host: " << socket3.getAddress().getIPAddress() << std::endl;
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  // TCPSocket tcpSocket(AddressFamily::IPv4);
  // UDPSocket udpSocket(AddressFamily::IPv6);
  // tcpSocket.bind(Address("127.0.0.1", 5555));
  // udpSocket.bind(Address("::1", 5556));
  // udpSocket.sendTo(Address("127.0.0.1", 5556), reinterpret_cast<const void*>("hello"), 5);
  return 0;
}
