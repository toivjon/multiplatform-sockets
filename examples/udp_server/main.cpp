#include "mps.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto SocketPort = 56789;

int main() {
  try {
    UDPSocket socket(SocketPort, AddressFamily::IPv6);
    std::cout << "Server IP=" << socket.getLocalIP() << " port=" << socket.getLocalPort() << std::endl;
    
    auto packet = socket.receive(BufferSize);
    Bytes buffer(packet.data);
    buffer.push_back('\0');
    std::cout << "response: " << reinterpret_cast<const char*>(&buffer[0]) << std::endl;
    socket.send(packet);
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
