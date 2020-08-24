#include "mps/udp_socket.h"
#include "mps/exception.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto SocketPort = 56789;

int main() {
  try {
    UDPSocket socket(SocketPort);
    std::cout << "Broadcase enabled before: " << socket.isBroadcastEnabled() << std::endl;
    socket.setBroadcastEnabled(true);
    std::cout << "Broadcase enabled after: " << socket.isBroadcastEnabled() << std::endl;

    auto addr = socket.getAddress();
    std::cout << "Bound addr='" << addr.getIPAddress() << "' port=" << addr.getPort() << std::endl;
    auto packet = socket.receive(BufferSize);
    std::cout << "echoing packet: " << packet.toString() << std::endl;
    socket.send(packet);
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
