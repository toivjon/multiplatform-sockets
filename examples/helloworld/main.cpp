#include "mps.h"

#include <iostream>

using namespace mps;

int main() {
  try {
    UDPSocket socket;
    socket.setBlocking(false);
    socket.setRouting(true);
    socket.setBroadcasting(true);
    socket.setSendBufferSize(1024);
    socket.setReceiveBufferSize(1024);
    socket.send({ {"127.0.0.1", 12345}, {'a'} });
    socket.send({ {"127.0.0.1", 12345}, {'a'} }, {UDPSendFlag::DontRoute});

    Address address;
    // TODO address.asSockaddr
    // address.getAddressFamily
  } catch (const SocketException& e) {
    std::cout << e.what() << std::endl;
  }

  /*
  try {
    UDPSocket socket;
    std::cout << "1 Bound to port: " << socket.getAddress().getPort() << std::endl;
    std::cout << "1 Bound to host: " << socket.getAddress().getIPAddress() << std::endl;
    UDPSocket socket2(56789);
    std::cout << "2 Bound to port: " << socket2.getAddress().getPort() << std::endl;
    std::cout << "2 Bound to host: " << socket2.getAddress().getIPAddress() << std::endl;
    UDPSocket socket3(true);
    socket3.setBroadcastEnabled(true);
    std::cout << "3 Bound to port: " << socket3.getAddress().getPort() << std::endl;
    std::cout << "3 Bound to host: " << socket3.getAddress().getIPAddress() << std::endl;
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  */
  // TCPSocket tcpSocket(AddressFamily::IPv4);
  // UDPSocket udpSocket(AddressFamily::IPv6);
  // tcpSocket.bind(Address("127.0.0.1", 5555));
  // udpSocket.bind(Address("::1", 5556));
  // udpSocket.sendTo(Address("127.0.0.1", 5556), reinterpret_cast<const void*>("hello"), 5);
  return 0;
}
