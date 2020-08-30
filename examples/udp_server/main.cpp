#include "mps.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto SocketPort = 56789;

int main() {
  try {
    UDPSocket socket(SocketPort);
    // auto addr = socket.getAddress();
    // std::cout << "Bound addr='" << addr.getIPAddress() << "' port=" << addr.getPort() << std::endl;
    auto packet = socket.receive(BufferSize);
    Bytes buffer(packet.getData());
    buffer.push_back('\0');
    std::cout << "response: " << reinterpret_cast<const char*>(&buffer[0]) << std::endl;
    socket.send(packet);
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
