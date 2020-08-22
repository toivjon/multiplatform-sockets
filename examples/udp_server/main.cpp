#include "mps/new_udp_socket.h"
#include "mps/exception.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto SocketPort = 56789;

int main() {
  try {
    UDPSocket socket(SocketPort);
    auto packet = socket.recv(BufferSize);
    std::cout << "echoing packet: " << packet.toString() << std::endl;
    socket.send(packet);
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
