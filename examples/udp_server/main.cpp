#include "mps.h"

#include <iostream>

using namespace mps;

constexpr auto SocketAddressFamily = AddressFamily::IPv4;
constexpr auto SocketPort = 56789;

int main() {
  try {
    // build and bind a new UDP socket and print out the socket info.
    UDPSocket socket(SocketAddressFamily, SocketPort);
    std::cout << "Bound a new UDP socket with following details:" << std::endl;
    std::cout << "    local-ip: " << socket.getLocalIP() << std::endl;
    std::cout << "  local-port: " << socket.getLocalPort() << std::endl;
    std::cout << "    blocking: " << socket.isBlocking() << std::endl;
    std::cout << "     routing: " << socket.isRouting() << std::endl;
    std::cout << "broadcasting: " << socket.isBroadcasting() << std::endl;
    std::cout << " recvBufSize: " << socket.getReceiveBufferSize() << std::endl;
    std::cout << " sendBufSize: " << socket.getSendBufferSize() << std::endl;
    std::cout << " recvTimeout: " << socket.getReceiveTimeout() << std::endl;

    // wait for incoming data by blocking and then print datagram info.
    std::cout << "Waiting for incoming UDP datagram..." << std::endl;
    UDPPacket packet;
    socket.receive(packet);
    std::cout << "Received a UDP datagram with following details:" << std::endl;
    std::cout << "  remote-ip: " << packet.getIP() << std::endl;
    std::cout << "remote-port: " << packet.getPort() << std::endl;
    std::cout << "       data: " << &packet.getData()[0] << std::endl;

    // echo the received data back to caller to inform that we did receive it.
    std::cout << "Echoing the data back to client and closing..." << std::endl;
    socket.send(packet);
  } catch (const SocketException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
