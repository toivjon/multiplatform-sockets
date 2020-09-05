#include "mps.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto SocketAddressFamily = AddressFamily::IPv6;
constexpr auto SocketPort = 56789;

int main() {
  try {
    // build and bind a new UDP socket and print out the socket info.
    UDPSocket socket(SocketPort, SocketAddressFamily);
    std::cout << "Bound a new UDP socket with following details:" << std::endl;
    std::cout << "    local-ip: " << socket.getLocalIP() << std::endl;
    std::cout << "  local-port: " << socket.getLocalPort() << std::endl;
    std::cout << "    blocking: " << socket.isBlocking() << std::endl;
    std::cout << "     routing: " << socket.isRouting() << std::endl;
    std::cout << "broadcasting: " << socket.isBroadcasting() << std::endl;
    std::cout << " recvBufSize: " << socket.getReceiveBufferSize() << std::endl;
    std::cout << " sendBufSize: " << socket.getSendBufferSize() << std::endl;

    auto packet = socket.receive(BufferSize);
    std::cout << "response: " << reinterpret_cast<const char*>(&packet.data[0]) << std::endl;
    socket.send(packet);
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
